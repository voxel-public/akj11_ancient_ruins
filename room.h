// Room drawing and logic
#ifndef ROOM_H
#define ROOM_H

#define ROOM_DATA_WIDTH 15
#define ROOM_DATA_HEIGHT 13
#define ROOM_HEIGHT 208
#define ROOM_WIDTH 240
#define ROOM_MAP_WIDTH 3
#define ROOM_MAP_HEIGHT 3

extern char room_data[ROOM_DATA_WIDTH * ROOM_DATA_HEIGHT];
extern const char* room_map[];

//Draw current room data to screen, assuming PPU off
void room_draw( void );
void room_load( const char source[] );
char room_is_point_clear( char x, char y );
char room_is_square_clear( char x, char y, char size );
void room_load_current( void );
#endif