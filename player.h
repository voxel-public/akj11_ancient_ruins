#ifndef PLAYER_H
#define PLAYER_H

extern char player_pos_x, player_pos_y;

void player_set_position( char x, char y );
void player_tick( char pad );

#endif
