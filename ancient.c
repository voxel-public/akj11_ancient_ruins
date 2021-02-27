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
  0x0F,			// screen color

  0x0C,0x39,0x29,0x00,	// background palette 0
  0x1C,0x20,0x2C,0x00,	// background palette 1
  0x00,0x10,0x20,0x00,	// background palette 2
  0x06,0x16,0x26,0x00,   // background palette 3

  0x16,0x35,0x24,0x00,	// sprite palette 0
  0x00,0x37,0x25,0x00,	// sprite palette 1
  0x0D,0x2D,0x3A,0x00,	// sprite palette 2
  0x0D,0x27,0x2A	// sprite palette 3
};

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
  // draw test room
  room_draw_test();
  // enable rendering
  ppu_on_all();
  // infinite loop
  player_set_position( 32, 32 );  
  while(1) {
    pad = pad_poll( 0 );
    player_tick( pad );
    oam_id = 0;
    //draw player
    oam_id = oam_spr( player_pos_x, player_pos_y, 0xE0, 0x00, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y, 0xE2, 0x00, oam_id );
    oam_id = oam_spr( player_pos_x, player_pos_y+8, 0xE1, 0x00, oam_id );
    oam_id = oam_spr( player_pos_x+8, player_pos_y+8, 0xE3, 0x00, oam_id );
    //Hide any extra unused oam
    oam_hide_rest( oam_id );
    ppu_wait_frame();
  }
}
