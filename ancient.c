
#include "ancient.h"
#include "plan.h"
//Ancient ruins
//Alakajam 11 entry by voxel
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// BCD arithmetic support
#include "bcd.h"
//#link "bcd.c"

// VRAM update buffer
#include "vrambuf.h"
//#link "vrambuf.c"

#include "room.h"
//#link "room.c"
#include "player.h"
//#link "player.c"

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x08,			// screen color

  0x0D,0x39,0x29,0x00,	// background palette 0
  0x1C,0x20,0x2C,0x00,	// background palette 1
  0x00,0x10,0x20,0x00,	// background palette 2
  0x06,0x16,0x26,0x00,   // background palette 3

  0x17,0x37,0x18,0x00,	// sprite palette 0
  0x05,0x15,0x30,0x00,	// sprite palette 1
  0x0D,0x2D,0x3A,0x00,	// sprite palette 2
  0x0D,0x27,0x2A	// sprite palette 3
};

char ancient_is_animation_frame;
char frame_count;
// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
}

void main(void)
{
  char oam_id, pad;
  setup_graphics();
  
  player_set_map_position( 0, 0 );
  player_set_position( 240/2, 200/2 );  
  room_load( room_map[0] );
  
  // enable rendering
  ppu_on_all();
  
  while(1) {
    ancient_is_animation_frame = frame_count % 8 == 0;
    ++frame_count;
    pad = pad_poll( 0 );
    player_tick( pad );
    oam_id = 0;
    //draw player
    oam_id = player_draw_oam( oam_id );
    //Hide any extra unused oam
    oam_hide_rest( oam_id );
    ppu_wait_frame();
  }
}

void ancient_player_left_room( char direction ){
  switch( direction ){
    case DIR_NORTH:
      player_set_position( player_pos_x, ROOM_HEIGHT - 2 );
      player_set_map_position( player_map_x, player_map_y - 1 );
      break;
    case DIR_SOUTH:
      player_set_position( player_pos_x, 2 );
      player_set_map_position( player_map_x, player_map_y + 1 );
      break;
    case DIR_EAST:
      player_set_position( 2, player_pos_y );
      player_set_map_position( player_map_x + 1, player_map_y );
      break;
    case DIR_WEST:
      player_set_position( ROOM_WIDTH - 2, player_pos_y );
      player_set_map_position( player_map_x - 1, player_map_y );
      break;
  }
  ppu_off();
  room_load_current();
  ppu_on_all();
}