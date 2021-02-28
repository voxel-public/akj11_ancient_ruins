#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_STATE_VISIBLE 1
#define PLAYER_STATE_SUBMERGED 2
#define PLAYER_STATE_DEAD 8
#define PLAYER_STATE_REFORMING 16
#define PLAYER_STATE_SLIDING 32

#define PALETTE_NORMAL 0x01
#define PALETTE_DEAD 0x02
#define PALETTE_SUBMERGED 0x03


extern char player_map_x, player_map_y;
extern char player_pos_x[], player_pos_y[];
extern char player_state;
extern char player_spawn_map_x, player_spawn_map_y, player_spawn_pos_x, player_spawn_pos_y;

void player_set_map_position( char x, char y );
void player_set_position( char x, char y );
void player_tick( char pad );
char player_draw_oam( char oam_id );
void player_set_state( char status );
void player_die( void );
void player_set_spawn_position( char map_x, char map_y, char pos_x, char pos_y );
#endif
