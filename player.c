
#include "player.h"
#include "neslib.h"
#include "room.h"
#include "ancient.h"
#include "mapview.h"
#include "pickups.h"

#define PLAYER_DASH_SPEED 6
#define PLAYER_DASH_DURATION 10
#define PLAYER_ANIMATION_FRAMECOUNT 3
#define PLAYER_LEAD_X_NEGATIVE -6
#define PLAYER_LEAD_X_POSITIVE 0
#define PLAYER_LEAD_Y_NEGATIVE -2
#define PLAYER_LEAD_Y_POSITIVE 0

#define TILE_PLAYER_SHADOW 0xB0

char player_facing_right = true;
char player_slide_x, player_slide_y;
char player_animation_frame = 0;
char player_pos_x[5], player_pos_y[5];
char player_map_x, player_map_y;
char player_prev_tx, player_prev_ty;
char player_state;
char player_spawn_map_x, player_spawn_map_y, player_spawn_pos_x, player_spawn_pos_y;
char player_dash_duration;

void player_set_map_position( char x, char y ){
  player_map_x = x % ROOM_MAP_WIDTH;
  player_map_y = y % ROOM_MAP_HEIGHT;
}

void player_set_position( char x, char y ){
  char i = 0;
  while( i < 5 ){
    player_pos_x[i] = x;
    player_pos_y[i] = y;
    ++i;
  }
}

void player_align_x(){
  player_pos_x[0] = ( ( player_pos_x[0] >> 4 ) << 4 ) + 8;
}

void player_align_y(){
  player_pos_y[0] = ( ( player_pos_y[0] >> 4 ) << 4 ) + 8;
}

void player_tick( char pad ){
  char i;
  char tx, ty;
  char future_x;
  char future_y;
  char lead_x = 0;
  char lead_y = 0;
  char is_moving = false;
  signed char dx, dy;
  char tile;
  
  //Update trail positions
  i = 4;
  while( i >= 1 ){
    player_pos_x[i] = player_pos_x[i-1];
    player_pos_y[i] = player_pos_y[i-1];
    --i; 
  }
  
  if ( player_state & PLAYER_STATE_SLIDING ) { 
    if ( player_slide_x == 0 && player_slide_y == 0 ){ //sanity check
      player_set_state( player_state & ~PLAYER_STATE_SLIDING );
    }
    dx = player_slide_x;
    dy = player_slide_y;
  } else if ( pad & PAD_B && pickup_collected_dash ){ // Dash?
    if ( pad & PAD_LEFT ){
      dx = -PLAYER_DASH_SPEED;
      player_facing_right = false;
      is_moving = true;
    }
    if ( pad & PAD_RIGHT ){
      dx = PLAYER_DASH_SPEED;
      player_facing_right = true;
      is_moving = true;
    }
    if ( pad & PAD_UP ){
      dy = -PLAYER_DASH_SPEED;
      is_moving = true;
    }
    if ( pad & PAD_DOWN ){
      dy = PLAYER_DASH_SPEED;
      is_moving = true;
    }
  } else {  //Normal movement
    //Player control input
    if ( pad & PAD_LEFT ){
      dx = -1;
      lead_x = PLAYER_LEAD_X_NEGATIVE;
      player_facing_right = false;
      is_moving = true;
    }
    if ( pad & PAD_RIGHT ){
      dx = 1;
      lead_x = PLAYER_LEAD_X_POSITIVE;
      player_facing_right = true;
      is_moving = true;
    }
    if ( pad & PAD_UP ){
      dy = -1;
      lead_y = PLAYER_LEAD_Y_NEGATIVE;
      is_moving = true;
    }
    if ( pad & PAD_DOWN ){
      dy = 1;
      lead_y = PLAYER_LEAD_Y_POSITIVE;
      is_moving = true;
    }
  }
  if ( pickup_collected_map && ( pad & PAD_SELECT ) ){
    mapview();
    return;
  }
  //Early exit and skip movement code for dead player
  if ( player_state & ( PLAYER_STATE_DEAD | PLAYER_STATE_REFORMING ) ){
    return;
  }
  //Check if the player has stepped onto a new tile
  tx = ( player_pos_x[0] >> 4 ) * 2 + 1;
  ty = ( player_pos_y[0] >> 4 ) * 2 + 1;
  if ( tx != player_prev_tx || ty != player_prev_ty ){
    tile = room_get_tile_at_pixel( player_pos_x[0], player_pos_y[0] );
    if ( tile == TILE_SPAWN ){
      if ( player_map_x != player_spawn_map_x || player_map_y != player_spawn_map_y ){
        ancient_screen_flash();
        player_set_spawn_position( player_map_x, player_map_y, player_pos_x[0], player_pos_y[0] );
        room_activate_spawn_at_pixel( player_pos_x[0], player_pos_y[0] );
      }
    }
    if ( tile == TILE_SWITCH ){
      ancient_toggle_barrier();
    }
    if ( tile == TILE_DIR_N ){
      player_slide_x = 0;
      player_slide_y = -1;
      player_set_state( player_state | PLAYER_STATE_SLIDING );
      player_align_x();
    } else if ( tile == TILE_DIR_E ){
      player_slide_x = 1;
      player_slide_y = 0;
      player_set_state( player_state | PLAYER_STATE_SLIDING );
      player_align_y();
    } else if ( tile == TILE_DIR_S ){
      player_slide_x = 0;
      player_slide_y = 1;
      player_set_state( player_state | PLAYER_STATE_SLIDING );
      player_align_x();
    } else if ( tile == TILE_DIR_W ){
      player_slide_x = -1;
      player_slide_y = 0;
      player_set_state( player_state | PLAYER_STATE_SLIDING );
      player_align_y();
    } else if ( tile == TILE_DIR_SLIDE ){
      if ( !( player_state & PLAYER_STATE_SLIDING ) ){
        if ( dy > 0 ){
          player_slide_y = 1;
          player_slide_x = 0;
          player_align_x();
        } else if ( dy < 0 ){
          player_slide_y = -1;
          player_slide_x = 0;
          player_align_x();
        } else if ( dx > 0 ) {
          player_slide_y = 0;
          player_slide_x = 1;
          player_align_y();
        } else {
          player_slide_y = 0;
          player_slide_x = -1;
          player_align_y();
        }
        player_set_state( player_state | PLAYER_STATE_SLIDING );
      }
    } else { //not sliding
      player_slide_x = 0;
      player_slide_y = 0;
    }
  }
  
  
  //Movement code has ended up a bit weird i'll admit
  //Apply y movement
  while( dy != 0 ){
    if ( dy < 0 ) { //'negative'
      future_y = player_pos_y[0] - 1;
      dy += 1;  
    } else {
      future_y = player_pos_y[0] + 1;
      dy -= 1;  
    }
    if ( room_is_square_clear( player_pos_x[0], future_y+lead_y-4, 6 ) ){
   	player_pos_y[0] = future_y;
    }
  }
  //Apply x movement
  while( dx != 0 ){
    if ( dx < 0 ) { //'negative'
      future_x = player_pos_x[0] - 1;
      dx += 1;  
    } else {
      future_x = player_pos_x[0] + 1;
      dx -= 1;  
    }
    if ( room_is_square_clear( future_x+lead_x, player_pos_y[0]-4, 6 ) ){
      player_pos_x[0] = future_x;
    }
  }
  

  //Exit rooms
  if ( player_pos_x[0] < 7 )
    ancient_player_left_room( DIR_WEST );
  else if ( player_pos_x[0] > ROOM_WIDTH-7 )
    ancient_player_left_room( DIR_EAST );
  else if ( player_pos_y[0] < 7 )
    ancient_player_left_room( DIR_NORTH );
  else if ( player_pos_y[0] > ROOM_HEIGHT-7 )
    ancient_player_left_room( DIR_SOUTH );
  //Detect water/dry
  if ( room_get_tile_at_pixel( player_pos_x[0], player_pos_y[0] ) == TILE_WATER ){
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
  

  player_prev_tx = tx;
  player_prev_ty = ty;
  
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
  oam_id = oam_spr( player_pos_x[0]-count, player_pos_y[0]-8-count, 0xD8, palette, oam_id );
  oam_id = oam_spr( player_pos_x[0]+8+count, player_pos_y[0]-8-count, 0xD8+2, palette, oam_id );
  oam_id = oam_spr( player_pos_x[0]-count, player_pos_y[0]+count, 0xD8+1, palette, oam_id );
  oam_id = oam_spr( player_pos_x[0]+8+count, player_pos_y[0]+count, 0xD8+3, palette, oam_id );
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
  oam_id = oam_spr( player_pos_x[0]-count, player_pos_y[0]-8-count, 0xD8, palette, oam_id );
  oam_id = oam_spr( player_pos_x[0]+8+count, player_pos_y[0]-8-count, 0xD8+2, palette, oam_id );
  oam_id = oam_spr( player_pos_x[0]-count, player_pos_y[0]+count, 0xD8+1, palette, oam_id );
  oam_id = oam_spr( player_pos_x[0]+8+count, player_pos_y[0]+count, 0xD8+3, palette, oam_id );
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
  //Player is doing OK somehow, draw them!
  if ( player_facing_right ){
    oam_id = oam_spr( player_pos_x[0], player_pos_y[0]-8, base_index, palette_top, oam_id );
    oam_id = oam_spr( player_pos_x[0]+8, player_pos_y[0]-8, base_index+2, palette_top, oam_id );
    oam_id = oam_spr( player_pos_x[0], player_pos_y[0], base_index+1, palette_bottom, oam_id );
    oam_id = oam_spr( player_pos_x[0]+8, player_pos_y[0], base_index+3, palette_bottom, oam_id );
  } else {
    oam_id = oam_spr( player_pos_x[0]+8, player_pos_y[0]-8, base_index, palette_top | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x[0], player_pos_y[0]-8, base_index+2, palette_top | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x[0]+8, player_pos_y[0], base_index+1, palette_bottom | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x[0], player_pos_y[0], base_index+3, palette_bottom | OAM_FLIP_H, oam_id );
  }
  if ( pickup_collected_dash ){
    //Dash trail A
    if ( player_facing_right ){
      oam_id = oam_spr( player_pos_x[2], player_pos_y[2]-8, base_index, PALETTE_DEAD, oam_id );
      oam_id = oam_spr( player_pos_x[2]+8, player_pos_y[2]-8, base_index+2, PALETTE_DEAD, oam_id );
      oam_id = oam_spr( player_pos_x[2], player_pos_y[2], base_index+1, PALETTE_DEAD, oam_id );
      oam_id = oam_spr( player_pos_x[2]+8, player_pos_y[2], base_index+3, PALETTE_DEAD, oam_id );
    } else {
      oam_id = oam_spr( player_pos_x[2]+8, player_pos_y[2]-8, base_index, PALETTE_DEAD | OAM_FLIP_H, oam_id );
      oam_id = oam_spr( player_pos_x[2], player_pos_y[2]-8, base_index+2, PALETTE_DEAD | OAM_FLIP_H, oam_id );
      oam_id = oam_spr( player_pos_x[2]+8, player_pos_y[2], base_index+1, PALETTE_DEAD | OAM_FLIP_H, oam_id );
      oam_id = oam_spr( player_pos_x[2], player_pos_y[2], base_index+3, PALETTE_DEAD | OAM_FLIP_H, oam_id );
    }
    //Dash trail B
    if ( player_facing_right ){
      oam_id = oam_spr( player_pos_x[4], player_pos_y[4]-8, base_index, PALETTE_DEAD, oam_id );
      oam_id = oam_spr( player_pos_x[4]+8, player_pos_y[4]-8, base_index+2, PALETTE_DEAD, oam_id );
      oam_id = oam_spr( player_pos_x[4], player_pos_y[4], base_index+1, PALETTE_DEAD, oam_id );
      oam_id = oam_spr( player_pos_x[4]+8, player_pos_y[4], base_index+3, PALETTE_DEAD, oam_id );
    } else {
      oam_id = oam_spr( player_pos_x[4]+8, player_pos_y[4]-8, base_index, PALETTE_DEAD | OAM_FLIP_H, oam_id );
      oam_id = oam_spr( player_pos_x[4], player_pos_y[4]-8, base_index+2, PALETTE_DEAD | OAM_FLIP_H, oam_id );
      oam_id = oam_spr( player_pos_x[4]+8, player_pos_y[4], base_index+1, PALETTE_DEAD | OAM_FLIP_H, oam_id );
      oam_id = oam_spr( player_pos_x[4], player_pos_y[4], base_index+3, PALETTE_DEAD | OAM_FLIP_H, oam_id );
    }
  }
  //Shadow!
  if ( !( player_state & PLAYER_STATE_SUBMERGED ) ) {
    oam_id = oam_spr( player_pos_x[0], player_pos_y[0] + 4, TILE_PLAYER_SHADOW, 0x02 | OAM_BEHIND, oam_id );
    oam_id = oam_spr( player_pos_x[0]+8, player_pos_y[0] + 4, TILE_PLAYER_SHADOW, 0x02 | OAM_FLIP_H | OAM_BEHIND, oam_id );
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


