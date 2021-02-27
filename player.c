#include "player.h"
#include "neslib.h"
#include "room.h"
#include "ancient.h"

#define PLAYER_ANIMATION_FRAMECOUNT 3
#define TILE_SHADOW 0xB0

char player_facing_right = true;
char player_animation_frame = 0;
char player_pos_x, player_pos_y;
char player_map_x, player_map_y;

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
    lead_x = -4;
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
    lead_y = -4;
    is_moving = true;
  }
  if ( pad & PAD_DOWN ){
    future_y += 1;
    lead_y = 4;
    is_moving = true;
  }
  
  //Check for collisions and apply movement
  if ( room_is_square_clear( player_pos_x, future_y+lead_y, 6 ) ){
    player_pos_y = future_y;
  }
  if ( room_is_square_clear( future_x+lead_x, player_pos_y, 6 ) ){
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
  
  //Animation
  if ( ancient_is_animation_frame ){
    if ( is_moving ){
      player_animation_frame = player_animation_frame > 0 ? player_animation_frame - 1 : 3;
    }
    else
      player_animation_frame = 1;
  }
  
}

char player_draw_oam( char oam_id ){
  char base_index;
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


  //Player character
  if ( player_facing_right ){
    oam_id = oam_spr( player_pos_x, player_pos_y, base_index, 0x01, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y, base_index+2, 0x01, oam_id );
    oam_id = oam_spr( player_pos_x, player_pos_y+8, base_index+1, 0x01, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y+8, base_index+3, 0x01, oam_id );
  } else {
    oam_id = oam_spr( player_pos_x+8, player_pos_y, base_index, 0x01 | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x, player_pos_y, base_index+2, 0x01 | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y+8, base_index+1, 0x01 | OAM_FLIP_H, oam_id );
    oam_id = oam_spr( player_pos_x, player_pos_y+8, base_index+3, 0x01 | OAM_FLIP_H, oam_id );
  }
  //Shadow
  oam_id = oam_spr( player_pos_x, player_pos_y + 12, TILE_SHADOW, 0x02 | OAM_BEHIND, oam_id );
  oam_id = oam_spr( player_pos_x+8, player_pos_y + 12, TILE_SHADOW, 0x02 | OAM_FLIP_H | OAM_BEHIND, oam_id );
  return oam_id;
}