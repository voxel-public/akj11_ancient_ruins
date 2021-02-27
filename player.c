#include "player.h"
#include "neslib.h"


char player_pos_x, player_pos_y;

void player_set_position( char x, char y ){
  player_pos_x = x;
  player_pos_y = y;
}

void player_tick( char pad ){
  if ( pad & PAD_LEFT )
    player_pos_x -= 1;
  if ( pad & PAD_RIGHT )
    player_pos_x += 1;
   if ( pad & PAD_UP )
    player_pos_y -= 1;
  if ( pad & PAD_DOWN )
    player_pos_y += 1;
}