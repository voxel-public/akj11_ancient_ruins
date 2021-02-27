#include "player.h"
#include "neslib.h"
#include "room.h"

char player_pos_x, player_pos_y;

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
  
  if ( player_pos_x < 1 || player_pos_x > 239 || player_pos_y < 1 || player_pos_y > 207 ){
    player_pos_x = (256/2);
    player_pos_y = (240/2);
  }
}