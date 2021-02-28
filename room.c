#include "room.h"
#include "neslib.h"
#include "vrambuf.h"
#include "player.h"
#include "mapdata.h"
#include "mapview.h"
#include "pickups.h"
#include "enemy.h"
#include "ancient.h"

#define TILE_PICKUP_MAP 26
#define TILE_PICKUP_DASH 23

#define TILE_ENEMY_WAYPOINT_1 36
#define TILE_ENEMY_WAYPOINT_2 37
#define TILE_ENEMY_WAYPOINT_3 38
#define TILE_ENEMY_WAYPOINT_4 39
#define TILE_ENEMY_WAYPOINT_5 40
#define TILE_ENEMY_WAYPOINT_6 41

char room_data[ROOM_DATA_WIDTH * ROOM_DATA_HEIGHT];

const char* room_map[] = {
  map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_11_01, map_12_01, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_null, map_06_02, map_07_02, map_08_02, map_09_02, map_10_02, map_11_02, map_12_02, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_05_03, map_06_03, map_null, map_null, map_null, map_null, map_11_03, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_05_04, map_null, map_null, map_null, map_null, map_null, map_11_04, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_05_05, map_null, map_null, map_null, map_09_05, map_10_05, map_11_05, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_05_06, map_null, map_null, map_null, map_09_06, map_null, map_11_06, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_05_07, map_null, map_null, map_null, map_09_07, map_10_07, map_11_07, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_05_08, map_null, map_null, map_null, map_null, map_null, map_11_08, map_null, map_null, map_null, map_null,
  map_null, map_null, map_02_09, map_03_09, map_04_09, map_05_09, map_06_09, map_07_09, map_08_09, map_09_09, map_10_09, map_11_09, map_null, map_null, map_null, map_null,
  map_null, map_null, map_02_10, map_null, map_04_10, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null,
  map_null, map_null, map_02_11, map_03_11, map_04_11, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_04_12, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_04_13, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null,
  map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null, map_null,
};

//Note that visible onscreen space starts at 8x8
char room_is_point_clear( char x, char y ){
  char tx, ty, data_index;
  tx = x >> 4;
  ty = y >> 4;
  data_index = ty * ROOM_DATA_WIDTH + tx;
  return tx >= ROOM_DATA_WIDTH || ty >= ROOM_DATA_HEIGHT || room_data[data_index] != 1;
}

char room_is_square_clear( char x, char y, char size ){
  return room_is_point_clear( x, y ) && 
    room_is_point_clear( x + size, y ) && 
    room_is_point_clear( x, y + size ) &&
    room_is_point_clear( x + size, y + size );
}
//Note that visible onscreen space starts at 8x8
char room_is_point_wet( char x, char y ){
  char tx, ty, data_index;
  tx = x >> 4;
  ty = y >> 4;
  data_index = ty * ROOM_DATA_WIDTH + tx;
  return tx >= ROOM_DATA_WIDTH || ty >= ROOM_DATA_HEIGHT || room_data[data_index] == TILE_WATER;
}

char room_is_square_wet( char x, char y, char size ){
  return room_is_point_wet( x, y ) && 
    room_is_point_wet( x + size, y ) && 
    room_is_point_wet( x, y + size ) &&
    room_is_point_wet( x + size, y + size );
}


void room_load( const char source[] ){
  char i;
  i = 0;
  while( i < ROOM_DATA_WIDTH * ROOM_DATA_HEIGHT ){
    room_data[i] = source[i];
    ++i;
  }
  room_draw();
}

//Draw the currently loaded room to screen, assumes PPU is off
void room_draw( void ){
  char buffer[32*2]; //Two rows of nametable data to draw
  char x, y, data_index, prev_data_index, buffer_index, cell;
  char is_map_room, is_dash_room, is_enemy_room;
  //Assume no pickups are in this room until we find some
  is_map_room = false;
  is_dash_room = false;
  is_enemy_room = false;
  pickup_entered_room(); //Clears any pickups
  mapview_entered_room();
  enemy_deactivate();
  for ( y = 0; y < ROOM_DATA_HEIGHT; ++y ){
    for ( x = 0; x < ROOM_DATA_WIDTH; ++x ){
      data_index = y * ROOM_DATA_WIDTH + x;
      buffer_index = x * 2;
      cell = room_data[data_index];
      //Check if this cell is a pickup spawn
      //Draw it as a floor and set the pickup position appropriately
      if ( cell == TILE_PICKUP_MAP ){
        is_map_room = true;
        cell = TILE_FLOOR;
        pickup_entered_map_room( 8 + (x<<4), 8 + (y<<4) );
      }
      if ( cell == TILE_PICKUP_DASH ){
        is_dash_room = true;
        cell = TILE_FLOOR;
        pickup_entered_dash_room( 8 + (x<<4), 8 + (y<<4) );
      }
      if ( cell == TILE_SPAWN ){
        //is the spawn active?
        if ( player_map_x == player_spawn_map_x &&
          player_map_y == player_spawn_map_y ){
          buffer[buffer_index] = 0xEC;
          buffer[buffer_index+1] = 0xEE;
          buffer[buffer_index+32] = 0xED;
	  buffer[buffer_index+33] = 0xEF;
        } else {
          buffer[buffer_index] = 0xE8;
          buffer[buffer_index+1] = 0xEA;
          buffer[buffer_index+32] = 0xE9;
	  buffer[buffer_index+33] = 0xEB;
        }
      }
      if ( cell == TILE_BARRIER_A ){
        if ( ancient_is_barrier_active ){
          buffer[buffer_index] = 0x0C;
          buffer[buffer_index+1] = 0x0C;
          buffer[buffer_index+32] = 0x0C;
          buffer[buffer_index+32+1] = 0x0C;
          room_data[data_index] = TILE_WALL;
        } else {
          buffer[buffer_index] = 0x0B;
          buffer[buffer_index+1] = 0x0B;
          buffer[buffer_index+32] = 0x0B;
          buffer[buffer_index+32+1] = 0x0B;
          room_data[data_index] = TILE_FLOOR;
        }
      }
      if ( cell == TILE_BARRIER_B ){
        if ( !ancient_is_barrier_active ){
          buffer[buffer_index] = 0x0F;
          buffer[buffer_index+1] = 0x0F;
          buffer[buffer_index+32] = 0x0F;
          buffer[buffer_index+32+1] = 0x0F;
          room_data[data_index] = TILE_WALL;
        } else {
          buffer[buffer_index] = 0x0B;
          buffer[buffer_index+1] = 0x0B;
          buffer[buffer_index+32] = 0x0B;
          buffer[buffer_index+32+1] = 0x0B;
          room_data[data_index] = TILE_FLOOR;
        }
      }
      if ( cell == TILE_SWITCH ){
        if ( ancient_is_barrier_active ){
          buffer[buffer_index] = 0xC8;
          buffer[buffer_index+1] = 0xCA;
          buffer[buffer_index+32] = 0xC9;
          buffer[buffer_index+32+1] = 0xCB;
        } else {
          buffer[buffer_index] = 0xF0;
          buffer[buffer_index+1] = 0xF2;
          buffer[buffer_index+32] = 0xF1;
          buffer[buffer_index+32+1] = 0xF3;
        }
      }
      if ( cell == TILE_DIR_N ){
        buffer[buffer_index] = 0x83;
        buffer[buffer_index+1] = 0x84;
        buffer[buffer_index+32] = 0x83;
        buffer[buffer_index+32+1] = 0x84;
      }
      if ( cell == TILE_DIR_E ){
        buffer[buffer_index] = 0x84;
        buffer[buffer_index+1] = 0x84;
        buffer[buffer_index+32] = 0x83;
        buffer[buffer_index+32+1] = 0x83;
      }
      if ( cell == TILE_DIR_S ){
        buffer[buffer_index] = 0x84;
        buffer[buffer_index+1] = 0x83;
        buffer[buffer_index+32] = 0x84;
        buffer[buffer_index+32+1] = 0x83;
      }
      if ( cell == TILE_DIR_W ){
        buffer[buffer_index] = 0x83;
        buffer[buffer_index+1] = 0x83;
        buffer[buffer_index+32] = 0x84;
        buffer[buffer_index+32+1] = 0x84;
      }
      if ( cell == TILE_DIR_SLIDE ){
        buffer[buffer_index] = 0xF8;
        buffer[buffer_index+1] = 0xFA;
        buffer[buffer_index+32] = 0xF9;
        buffer[buffer_index+32+1] = 0xFB;
      }
      if ( cell == TILE_BEGIN ){
        cell = TILE_FLOOR;
      }
      //If this is an enemy waypoint, draw it as water and update the enemy waypoint list
      if ( cell >= TILE_ENEMY_WAYPOINT_1 && cell <= TILE_ENEMY_WAYPOINT_6 ){
        enemy_waypoints_x[cell-TILE_ENEMY_WAYPOINT_1] = 8 + (x<<4);
        enemy_waypoints_y[cell-TILE_ENEMY_WAYPOINT_1] = 8 + (y<<4);
        cell = TILE_WATER;
        room_data[data_index] = TILE_WATER;
        is_enemy_room = true;
      }
      if ( cell == TILE_FLOOR ){ //Empty floor tile
        //Check for walls to North or West and add shadows to floor if found
        buffer[buffer_index] = TILE_EMPTY;
        buffer[buffer_index+1] = TILE_EMPTY;
        buffer[buffer_index+32] = TILE_EMPTY;
        buffer[buffer_index+32+1] = TILE_EMPTY;
        if ( y > 0 ){
          prev_data_index = (y-1) * ROOM_DATA_WIDTH + x;
          buffer[buffer_index+1] = room_data[prev_data_index] == TILE_WALL ? TILE_SHADOW : TILE_EMPTY;
	  buffer[buffer_index] = room_data[prev_data_index] == TILE_WALL ? TILE_SHADOW : buffer[buffer_index];
        }
        if ( x > 0 ){
          prev_data_index = y * ROOM_DATA_WIDTH + ( x - 1 );
          buffer[buffer_index+32] = room_data[prev_data_index] == TILE_WALL ? TILE_SHADOW : TILE_EMPTY;
          buffer[buffer_index] = room_data[prev_data_index] == TILE_WALL ? TILE_SHADOW : buffer[buffer_index];
        }
        if ( x > 0 && y > 0 ){
          prev_data_index = ( y - 1 ) * ROOM_DATA_WIDTH + ( x - 1 );
          buffer[buffer_index] = room_data[prev_data_index] == TILE_WALL ? TILE_SHADOW : buffer[buffer_index];
        }
        
      } else if ( cell == TILE_WALL ){
        buffer[buffer_index] = 0xF4;
        buffer[buffer_index+1] = 0xF6;
        buffer[buffer_index+32] = 0xF5;
        buffer[buffer_index+32+1] = 0xF7;
      } else if ( cell == TILE_WATER ){
        buffer[buffer_index] = 0x03;
        buffer[buffer_index+1] = 0x03;
        buffer[buffer_index+32] = 0x03;
        buffer[buffer_index+32+1] = 0x03;
      }
    }
    vram_adr( NTADR_A( 1, 1+y*2 ) );
    vram_write( buffer, 64 );
  }
  vram_adr( NTADR_A( 1, 27 ) );
  vram_fill( TILE_SHADOW, 32 );
  if ( is_enemy_room )
    enemy_activate();
}

void room_load_current( void ){
  char index = player_map_x + player_map_y * ROOM_MAP_WIDTH;
  room_load( room_map[index] );
}

char room_get_tile_at_pixel( char x, char y ){
  char tx, ty, data_index;
  tx = x >> 4;
  ty = y >> 4;	 
  data_index = ty * ROOM_DATA_WIDTH + tx;
  return ( tx >= ROOM_DATA_WIDTH || ty >= ROOM_DATA_HEIGHT ) ? 0x00 : room_data[data_index];
}

void room_activate_spawn_at_pixel( char x, char y ){
  char tx, ty;
  char bytes_top[] = { 0xEC, 0xEE };
  char bytes_bottom[] = { 0xED, 0xEF };
  //convert from pixel position to onscreen tiles
  tx = ( x >> 4 ) * 2 + 1;
  ty = ( y >> 4 ) * 2 + 1;	 
  vrambuf_put( NTADR_A( tx, ty ), bytes_top, 2 );
  vrambuf_put( NTADR_A( tx, ty + 1 ), bytes_bottom, 2 );
}