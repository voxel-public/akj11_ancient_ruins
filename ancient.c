
#include "plan.h"

#include "ancient.h"
//#include "plan.h" 
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

#include "bcd.h"
//#link "bcd.c"

// VRAM update buffer
#include "vrambuf.h"
//#link "vrambuf.c"

// link the pattern table into CHR ROM
//#link "chr_generic.s"

//#link "famitone2.s"
//#link "music.s"
extern const char music_data[];
//#link "sfx.s"
extern const char sfx_data[];


#include "room.h"
//#link "room.c"
#include "player.h"
//#link "player.c"
#include "mapdata.h"
//#link "mapdata.c"
#include "pickups.h"
//#link "pickups.c"

#include "mapview.h"
//#link "mapview.c"

#include "enemy.h"
//#link "enemy.c"


/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x07,			// screen color

  0x11,0x28,0x0F,0x00,	// background palette 0
  0x1C,0x20,0x2C,0x00,	// background palette 1
  0x03,0x13,0x23,0x00,	// background palette 2
  0x06,0x16,0x26,0x00,   // background palette 3

  0x17,0x37,0x18,0x00,	// sprite palette 0
  0x05,0x15,0x30,0x00,	// sprite palette 1
  0x0D,0x00,0x10,0x00,	// sprite palette 2
  0x02,0x02,0x02	// sprite palette 3
};
const char FLASH_PALETTE[32] = { 
  0x30,			

  0x30,0x30,0x30,0x30,
  0x30,0x30,0x30,0x30,
  0x30,0x30,0x30,0x30,
  0x30,0x30,0x30,0x30,
  
  0x30,0x30,0x30,0x00,
  0x30,0x30,0x30,0x00,
  0x30,0x30,0x30,0x00,
  0x30,0x30,0x30
};

char ancient_is_barrier_active;
char ancient_is_animation_frame;
char ancient_frame_count;
char ancient_music_status;

// setup PPU and tables
void setup_graphics() {
  //clear any trash out of the vram buffer
  vrambuf_clear();
  set_vram_update( updbuf );
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
}
// set up famitone library
void setup_audio() {
  famitone_init( music_data );
  sfx_init( sfx_data );
  nmi_set_callback( famitone_update );
  ancient_music_status = 0;
}

void ancient_title_screen( void ){
  char pad;
  ppu_off();
  vram_adr( NTADR_A( 0, 0 ) );
  vram_fill( 0, 32*30 );
  vram_adr( NTADR_A( 3, 5 ) );
  vram_write( "ESCAPE FROM FLOODED RUINS", 25 );
  vram_adr( NTADR_A( 6, 11 ) );
  vram_write( "MADE IN 48 HOURS FOR", 20 );
  vram_adr( NTADR_A( 10, 13 ) );
  vram_write( "ALAKAJAM 11", 11 );
  vram_adr( NTADR_A( 12, 15 ) );
  vram_write( "BY VOXEL", 8 );
  vram_adr( NTADR_A( 11, 22 ) );
  vram_write( "PRESS START", 11 );
  ppu_on_all();
  while( true ){
    pad = pad_poll( 0 );
    if ( pad & PAD_START )
      break;
  }
}

void ancient_screen_flash( void ){
  char duration = 4;
  //set all colours to white
  pal_all( FLASH_PALETTE );
  //Wait a moment
  while( duration > 0 ){
    vrambuf_flush();
    --duration;
  }
  //restore palette
  pal_all( PALETTE );
}

void ancient_toggle_barrier( void ){
  char duration = 4;
  //set all colours to white
  pal_all( FLASH_PALETTE );
  vrambuf_flush();
  ppu_off();
  ancient_is_barrier_active = !ancient_is_barrier_active;
  room_load_current();
  ppu_on_all();
  //Wait a moment
  while( duration > 0 ){
    vrambuf_flush();
    --duration;
  }
  //restore palette
  pal_all( PALETTE );  
}

void main(void)
{
  char oam_id, pad;
  ppu_off();
    
  setup_audio();
  setup_graphics();
  
  ancient_title_screen();
  ppu_off();
  
  player_set_spawn_position( 4, 10, 120, 140 );
  player_set_state( PLAYER_STATE_VISIBLE );
  
  player_set_map_position( 4, 9 );
  player_set_position( 120, 60 );
  

  room_load_current();
  
  //Set initial barrier state
  ancient_is_barrier_active = true;
  
  //Dash testing
  //pickup_visible_dash = true;
  //pickup_x = 32;
  //pickup_y = 32;
  pickup_collected_dash = false;
  // enable rendering
  ppu_on_all();
  
  while(1) {
    ancient_is_animation_frame = ancient_frame_count % 8 == 0;
    ++ancient_frame_count;
    pad = pad_poll( 0 );
    player_tick( pad );
    pickup_collision_check();
    enemy_tick();
    oam_id = 0;
    //draw player
    oam_id = player_draw_oam( oam_id );
    //draw pickups
    oam_id = pickup_draw_oam( oam_id );
    //draw enemy
    oam_id = enemy_draw_oam( oam_id );
    //Hide any extra unused oam
    oam_hide_rest( oam_id );
    vrambuf_flush();
  }
}

void ancient_player_left_room( char direction ){
  switch( direction ){
    case DIR_NORTH:
      player_set_position( player_pos_x[0], ROOM_HEIGHT - 8 );
      player_set_map_position( player_map_x, player_map_y - 1 );
      break;
    case DIR_SOUTH:
      player_set_position( player_pos_x[0], 8 );
      player_set_map_position( player_map_x, player_map_y + 1 );
      break;
    case DIR_EAST:
      player_set_position( 8, player_pos_y[0] );
      player_set_map_position( player_map_x + 1, player_map_y );
      break;
    case DIR_WEST:
      player_set_position( ROOM_WIDTH - 8, player_pos_y[0] );
      player_set_map_position( player_map_x - 1, player_map_y );
      break;
  }
  ppu_off();
  room_load_current();
  ppu_on_all();
}

void ancient_player_dies( void ){
  if ( player_state & PLAYER_STATE_DEAD )
    return; //what is dead can never die
  ancient_frame_count = 0;
  sfx_play( 0, 0 );
  //sfx_play( 4, 0 );
  player_set_state( PLAYER_STATE_DEAD | PLAYER_STATE_VISIBLE );
}

void ancient_player_spawn_at_spawn_point( void ){
  ppu_off();
  player_set_map_position( player_spawn_map_x, player_spawn_map_y );
  player_set_position( player_spawn_pos_x, player_spawn_pos_y );
  room_load_current();
  player_set_state( PLAYER_STATE_VISIBLE | PLAYER_STATE_REFORMING );
  //sfx_play( 2, 0 );
  ppu_on_all();
  ancient_frame_count = 0;
}

void ancient_music_stop( void ){
  music_stop( );
}
void ancient_music_play_explore( void ){
  if ( ancient_music_status != 1 ){
    ancient_music_status = 1;
    music_stop();
  }
}
void ancient_music_play_danger( void ){
  if ( ancient_music_status != 2 ){
    ancient_music_status = 2;
    music_play( 1 );
  }
}