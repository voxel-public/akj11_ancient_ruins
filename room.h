// Room drawing and logic
#ifndef ROOM_H
#define ROOM_H

#define ROOM_DATA_WIDTH 15
#define ROOM_DATA_HEIGHT 13

extern char room_data[ROOM_DATA_WIDTH * ROOM_DATA_HEIGHT];

//Draw current room data to screen, assuming PPU off
void room_draw_test( void );
void room_load( const char source[] );
char room_is_point_clear( char x, char y );
char room_is_square_clear( char x, char y, char size );
#endif