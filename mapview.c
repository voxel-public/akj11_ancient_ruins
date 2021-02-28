#include "mapview.h"
#include "neslib.h"
#include "room.h"
#include "player.h"
#include "mapdata.h"

char mapview_mapdata[MAPVIEW_MAP_WIDTH * MAPVIEW_MAP_HEIGHT];

//Clear the map
void mapview_reset( void ){
  char count = 0;
  while( count < MAPVIEW_MAP_WIDTH * MAPVIEW_MAP_HEIGHT ){
    mapview_mapdata[count] = MAPVIEW_TILE_EMPTY;
    ++count;
  }
}

//Reveal all rooms to explored state
void mapview_cheat( void ){
  char x, y, index;
  y = 0;
  while( y < MAPVIEW_MAP_HEIGHT ){
    x = 0;
    while( x < MAPVIEW_MAP_WIDTH ){
      index = y * MAPVIEW_MAP_WIDTH + x;
      mapview_mapdata[index] = MAPVIEW_TILE_EXPLORED;
      ++x;
    }
    ++y;
  }
}

//switch to mapview mode
void mapview( void ){
  char pad, x, y, index, data;
  char buffer[32*2];
  
  //Draw the map
  ppu_off(); //We have to disable the screen to draw to it outside of vblank
  y = 0;
  while ( y < MAPVIEW_MAP_HEIGHT ){
    x = 0;
    while( x < MAPVIEW_MAP_WIDTH ){
      index = y * MAPVIEW_MAP_WIDTH + x;
      data = mapview_mapdata[index];
      if ( 
        ( x + MAPVIEW_DATA_OFFSET_X == player_map_x ) && 
        ( y + MAPVIEW_DATA_OFFSET_Y == player_map_y ) ) 
      {
        data = MAPVIEW_TILE_PLAYER;
      }
      //Draw some tiles to the row buffer
      switch( data ){
        default:
        case MAPVIEW_TILE_EMPTY:
          buffer[x*2] = 0;
          buffer[x*2+1] = 0;
          buffer[x*2+32] = 0;
          buffer[x*2+32+1] = 0;
          break;
        case MAPVIEW_TILE_SEEN:
	  buffer[x*2] = 0xCC;
          buffer[x*2+1] = 0xCE;
          buffer[x*2+32] = 0xCD;
          buffer[x*2+32+1] = 0xCF;
          break;
        case MAPVIEW_TILE_EXPLORED:
	  buffer[x*2] = 0xD0;
          buffer[x*2+1] = 0xD2;
          buffer[x*2+32] = 0xD1;
          buffer[x*2+32+1] = 0xD3;
          break;
        case MAPVIEW_TILE_PLAYER:
          buffer[x*2] = 0xD4;
          buffer[x*2+1] = 0xD6;
          buffer[x*2+32] = 0xD5;
          buffer[x*2+32+1] = 0xD7;
          break;
      }
      ++x;
    }
    //Copy the buffer to vram
    vram_adr( NTADR_A( 1, 1+y*2 ) );
    vram_write( buffer, 64 );
    ++y;
  }
  vram_adr( NTADR_A( 1, 27 ) );
  vram_fill( TILE_SHADOW, 32 );
  ppu_on_bg();
  
  //Wait until select is released otherwise we'll immediately exit the map
  while( true ){
    pad = pad_poll( 0 );
    if ( !( pad & PAD_SELECT ) ){
      break;
    }
  }
  //Wait until select is pressed again
  while( true ){
    pad = pad_poll( 0 );
    if ( pad & PAD_SELECT ){
      break;
    }
  }
  //Wait until select is released otherwise we'll immediately re-enter the map!
  while( true ){
    pad = pad_poll( 0 );
    if ( !( pad & PAD_SELECT ) ){
      break;
    }
  }
  ppu_off();
  //Restore the game screen
  //We don't do any resource swapping so we can just redraw the room as if we'd entered it
  room_draw();
  ppu_on_all();
}

//notify mapview that a room was entered, let it update the map
void mapview_entered_room( void ){
  char index, room_index;
  //Current room
  index = ( player_map_y - MAPVIEW_DATA_OFFSET_Y ) * MAPVIEW_MAP_WIDTH + ( player_map_x - MAPVIEW_DATA_OFFSET_X );
  mapview_mapdata[index] = MAPVIEW_TILE_EXPLORED;
  //Check for neighbouring rooms
  //This assumes the room map has a border of inaccessible but valid rooms around it
  if ( player_map_y < ROOM_MAP_HEIGHT && player_map_x < ROOM_MAP_WIDTH ){
    //N
    index = ( player_map_y - 1 - MAPVIEW_DATA_OFFSET_Y ) * MAPVIEW_MAP_WIDTH + ( player_map_x - MAPVIEW_DATA_OFFSET_X );
    room_index = ( player_map_y - 1 ) * ROOM_MAP_WIDTH + player_map_x;
    if ( room_map[room_index] != map_null ){
      mapview_mapdata[index] = mapview_mapdata[index] == MAPVIEW_TILE_EMPTY ? MAPVIEW_TILE_SEEN : mapview_mapdata[index];
    }
    //E
    index = ( player_map_y - MAPVIEW_DATA_OFFSET_Y ) * MAPVIEW_MAP_WIDTH + ( player_map_x + 1 - MAPVIEW_DATA_OFFSET_X );
    room_index = player_map_y * ROOM_MAP_WIDTH + player_map_x + 1;
    if ( room_map[room_index] != map_null ){
      mapview_mapdata[index] = mapview_mapdata[index] == MAPVIEW_TILE_EMPTY ? MAPVIEW_TILE_SEEN : mapview_mapdata[index];
    }
    //S
    index = ( player_map_y + 1 - MAPVIEW_DATA_OFFSET_Y ) * MAPVIEW_MAP_WIDTH + ( player_map_x - MAPVIEW_DATA_OFFSET_X );
    room_index = ( player_map_y + 1 ) * ROOM_MAP_WIDTH + player_map_x;
    if ( room_map[room_index] != map_null ){
      mapview_mapdata[index] = mapview_mapdata[index] == MAPVIEW_TILE_EMPTY ? MAPVIEW_TILE_SEEN : mapview_mapdata[index];
    }
    //W
    index = ( player_map_y - MAPVIEW_DATA_OFFSET_Y ) * MAPVIEW_MAP_WIDTH + ( player_map_x - 1 - MAPVIEW_DATA_OFFSET_X );
    room_index = ( player_map_y ) * ROOM_MAP_WIDTH + player_map_x - 1;
    if ( room_map[room_index] != map_null ){
      mapview_mapdata[index] = mapview_mapdata[index] == MAPVIEW_TILE_EMPTY ? MAPVIEW_TILE_SEEN : mapview_mapdata[index];
    }
  }
}