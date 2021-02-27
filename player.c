#include "player.h"
#include "neslib.h"


char player_pos_x, player_pos_y;

void player_set_position( char x, char y ){
  player_pos_x = x;
  player_pos_y = y;
}