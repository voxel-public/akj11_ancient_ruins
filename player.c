#include "player.h"
#include "neslib.h"
#include "room.h"
#include "ancient.h"

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
  if ( pad & PAD_LEFT ){
    future_x -= 1;
    lead_x = -4;
  }
  if ( pad & PAD_RIGHT ){
    future_x += 1;
    lead_x = 0;
  }
  if ( pad & PAD_UP ){
    future_y -= 1;
    lead_y = -4;
  }
  if ( pad & PAD_DOWN ){
    future_y += 1;
    lead_y = 2;
  }
  
  if ( room_is_square_clear( player_pos_x, future_y+lead_y, 5 ) ){
    player_pos_y = future_y;
  }
  if ( room_is_square_clear( future_x+lead_x, player_pos_y, 5 ) ){
    player_pos_x = future_x;
  }
  
  if ( player_pos_x <= 1 )
    ancient_player_left_room( DIR_WEST );
  else if ( player_pos_x >= ROOM_WIDTH )
    ancient_player_left_room( DIR_EAST );
  else if ( player_pos_y <= 1 )
    ancient_player_left_room( DIR_NORTH );
  else if ( player_pos_y >= ROOM_HEIGHT )
    ancient_player_left_room( DIR_SOUTH );
}