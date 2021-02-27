#ifndef PLAYER_H
#define PLAYER_H

extern char player_map_x, player_map_y;
extern char player_pos_x, player_pos_y;

void player_set_map_position( char x, char y );
void player_set_position( char x, char y );
void player_tick( char pad );
char player_draw_oam( char oam_id );
#endif
