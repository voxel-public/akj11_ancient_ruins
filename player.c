#include "player.h"
#include "neslib.h"
#include "room.h"
#include "ancient.h"

#define PLAYER_ANIMATION_FRAMECOUNT 3
#define TILE_PLAYER_SHADOW 0xB0

char player_facing_right = true;
char player_animation_frame = 0;
char player_pos_x, player_pos_y;
char player_map_x, player_map_y;
char player_state;
char player_spawn_map_x, player_spawn_map_y, player_spawn_pos_x, player_spawn_pos_y;


void player_set_map_position( char x, char y ){
  player_map_x = x % ROOM_MAP_WIDTH;
  player_map_y = y % ROOM_MAP_HEIGHT;
}

void player_set_position( char x, char y ){
  player_pos_x = x;
  player_pos_y = y;
}

void player_tick( char pad ){
  char future_x = player_pos_x;
  char future_y = player_pos_y;
  char lead_x = 0;
  char lead_y = 0;
  char is_moving = false;
  
  //Player control input
  if ( pad & PAD_LEFT ){
    future_x -= 1;
    lead_x = -6;
    player_facing_right = false;
    is_moving = true;
  }
  if ( pad & PAD_RIGHT ){
    future_x += 1;
    lead_x = 0;
    player_facing_right = true;
    is_moving = true;
  }
  if ( pad & PAD_UP ){
    future_y -= 1;
    lead_y = -2;
    is_moving = true;
  }
  if ( pad & PAD_DOWN ){
    future_y += 1;
    lead_y = 0;
    is_moving = true;
  }
  if ( pad & PAD_A ){
    ancient_player_dies();
  }
  if ( pad & PAD_B ){
    ancient_frame_count = 0;
    player_set_state( PLAYER_STATE_REFORMING | PLAYER_STATE_VISIBLE );
  }
  //Early exit and skip movement code for dead player
  if ( player_state & ( PLAYER_STATE_DEAD | PLAYER_STATE_REFORMING ) ){
    return;
  }
  
  //Check for collisions and apply movement
  if ( room_is_square_clear( player_pos_x, future_y+lead_y-4, 6 ) ){
    player_pos_y = future_y;
  }
  if ( room_is_square_clear( future_x+lead_x, player_pos_y-4, 6 ) ){
    player_pos_x = future_x;
  }
  
  //Exit rooms
  if ( player_pos_x <= 1 )
    ancient_player_left_room( DIR_WEST );
  else if ( player_pos_x >= ROOM_WIDTH )
    ancient_player_left_room( DIR_EAST );
  else if ( player_pos_y <= 1 )
    ancient_player_left_room( DIR_NORTH );
  else if ( player_pos_y >= ROOM_HEIGHT )
    ancient_player_left_room( DIR_SOUTH );
  //Detect water/dry
  if ( room_get_tile_at_pixel( player_pos_x, player_pos_y ) == TILE_WATER ){
    player_state = player_state | PLAYER_STATE_SUBMERGED;
  } else {
    player_state = player_state & ~PLAYER_STATE_SUBMERGED;
  }
  //Animation
  if ( ancient_is_animation_frame ){
    if ( is_moving ){
      player_animation_frame = player_animation_frame > 0 ? player_animation_frame - 1 : 3;
    }
    else
      player_animation_frame = 1;
  }
  
}

//Draw the play exploding to pieces and end the animation after 128 frames :)
char player_draw_oam_exploding( char oam_id ){
  char count, palette;
  count = ( ancient_frame_count > 30 ) ? ( ancient_frame_count - 30 ) * 4 : 0;
  if ( count >= 128 ){
    ancient_player_spawn_at_spawn_point();
    return oam_id;
  }
  palette = ancient_frame_count % 2 == 0 ? PALETTE_NORMAL : PALETTE_DEAD;
  oam_id = oam_spr( player_pos_x-count, player_pos_y-8-count, 0xD8, palette, oam_id );
  oam_id = oam_spr( player_pos_x+8+count, player_pos_y-8-count, 0xD8+2, palette, oam_id );
  oam_id = oam_spr( player_pos_x-count, player_pos_y+count, 0xD8+1, palette, oam_id );
  oam_id = oam_spr( player_pos_x+8+count, player_pos_y+count, 0xD8+3, palette, oam_id );
  return oam_id;
}

//Reverse of an explosion, the player reforms
char player_draw_oam_reforming( char oam_id ){
  char count, palette;
  count = ( 90-ancient_frame_count - 30 ) * 2;
  if ( count == 0 ){
    player_set_state( PLAYER_STATE_VISIBLE );
  }
  palette = ancient_frame_count % 2 == 0 ? PALETTE_NORMAL : PALETTE_DEAD;
  oam_id = oam_spr( player_pos_x-count, player_pos_y-8-count, 0xD8, palette, oam_id );
  oam_id = oam_spr( player_pos_x+8+count, player_pos_y-8-count, 0xD8+2, palette, oam_id );
  oam_id = oam_spr( player_pos_x-count, player_pos_y+count, 0xD8+1, palette, oam_id );
  oam_id = oam_spr( player_pos_x+8+count, player_pos_y+count, 0xD8+3, palette, oam_id );
  return oam_id;
}

char player_draw_oam( char oam_id ){
  
  char base_index, palette_top, palette_bottom;
  
  //Skip if player isnt visible
  if ( !( player_state & PLAYER_STATE_VISIBLE ) )
    return oam_id;
  
  switch( player_animation_frame ){
    case 0:
      base_index = 0xD8;
      break;
    case 2:
      base_index = 0xE0;
      break;
    case 1:
    case 3:
    default:
      base_index = 0xDC;
      break;
  }

  palette_top = 0x01;
  palette_bottom = ( player_state & PLAYER_STATE_SUBMERGED ) ? 0x03 : palette_top;
  //Player is exploding!
  if ( player_state & PLAYER_STATE_DEAD ){
    return player_draw_oam_exploding( oam_id );
  }
  //Player is reforming!
  if ( player_state & PLAYER_STATE_REFORMING ){
    return player_draw_oam_reforming( oam_id );
  }
  //Player is doing OK somehow!
  if ( player_facing_right ){
    oam_id = oam_spr( player_pos_x, player_pos_y-8, base_index, palette_top, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y-8, base_index+2, palette_top, oam_id );
    oam_id = oam_spr( player_pos_x, player_pos_y, base_index+1, palette_bottom, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y, base_index+3, palette_bottom, oam_id );
  } else {
    oam_id = oam_spr( player_pos_x+8, player_pos_y-8, base_index, palette_top | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x, player_pos_y-8, base_index+2, palette_top | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y, base_index+1, palette_bottom | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x, player_pos_y, base_index+3, palette_bottom | OAM_FLIP_H, oam_id );
  }
  //Shadow!
  if ( !( player_state & PLAYER_STATE_SUBMERGED ) ) {
    oam_id = oam_spr( player_pos_x, player_pos_y + 4, TILE_PLAYER_SHADOW, 0x02 | OAM_BEHIND, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y + 4, TILE_PLAYER_SHADOW, 0x02 | OAM_FLIP_H | OAM_BEHIND, oam_id );
  }
  return oam_id;
}
      
void player_set_state( char state ){
  player_state = state; 
}

void player_set_spawn_position( char map_x, char map_y, char pos_x, char pos_y ){
  player_spawn_map_x = map_x; 
  player_spawn_map_y = map_y;
  player_spawn_pos_x = pos_x;
  player_spawn_pos_y = pos_y;
}


