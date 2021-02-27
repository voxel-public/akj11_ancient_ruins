#include "room.h"
#include "neslib.h"
#include "player.h"

char room_data[ROOM_DATA_WIDTH * ROOM_DATA_HEIGHT];

const char room_test_data1[] = {
          1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1
        };
const char room_test_data2[] = {
  	  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
          0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
	  1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
	  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1
};
const char room_test_data3[] = {
  	  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
          0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0,
	  1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1
};
const char* room_map[] = {
  room_test_data1, room_test_data2, room_test_data1,
  room_test_data2, room_test_data3, room_test_data2,
  room_test_data1, room_test_data2, room_test_data1
};

//Note that visible onscreen space starts at 8x8
char room_is_point_clear( char x, char y ){
  char tx, ty, data_index;
  tx = x >> 4;
  ty = y >> 4;
  data_index = ty * ROOM_DATA_WIDTH + tx;
  return tx >= ROOM_DATA_WIDTH || ty >= ROOM_DATA_HEIGHT || room_data[data_index] == 0;
}

char room_is_square_clear( char x, char y, char size ){
  return room_is_point_clear( x, y ) && 
    room_is_point_clear( x + size, y ) && 
    room_is_point_clear( x, y + size ) &&
    room_is_point_clear( x + size, y + size );
}

void room_load( const char source[] ){
  char i = 0;
  while( i < ROOM_DATA_WIDTH * ROOM_DATA_HEIGHT ){
    room_data[i] = source[i];
    ++i;
  }
  room_draw();
}

//Draw the currently loaded room to screen, assumes PPU is off
void room_draw( void ){
  char buffer[32*2]; //Two rows of nametable data to draw
  char x, y, data_index, buffer_index, cell;
  
  for ( y = 0; y < ROOM_DATA_HEIGHT; ++y ){
    for ( x = 0; x < ROOM_DATA_WIDTH; ++x ){
      data_index = y * ROOM_DATA_WIDTH + x;
      buffer_index = x * 2;
      cell = room_data[data_index];
      if ( cell == 0 ){ //Empty
        buffer[buffer_index] = 0;
        buffer[buffer_index+1] = 0;
        buffer[buffer_index+32] = 0;
        buffer[buffer_index+32+1] = 0;
      } else if ( cell == 1 ){
        buffer[buffer_index] = 0xF4;
        buffer[buffer_index+1] = 0xF6;
        buffer[buffer_index+32] = 0xF5;
        buffer[buffer_index+32+1] = 0xF7;
      }
    }
    vram_adr( NTADR_A( 1, 1+y*2 ) );
    vram_write( buffer, 64 );
  }

}

void room_load_current( void ){
  char index = player_map_x + player_map_y * ROOM_MAP_WIDTH;
  room_load( room_map[index] );
}