#include "pickups.h"
#include "neslib.h"
#include "player.h"

char pickup_collected_map;
char pickup_collected_dash;
char pickup_visible_map;
char pickup_visible_dash;
char pickup_x, pickup_y; //Position of onscreen pickup (only one is ever visible)

//Called when entering a room with a pickup to start displaying it
void pickup_entered_map_room( char px, char py ){
  if ( pickup_collected_map )
    return;
  pickup_visible_map = true;
  pickup_visible_dash = false;
  pickup_x = px;
  pickup_y = py;
}
void pickup_entered_dash_room( char px, char py ){
  if ( pickup_collected_dash )
    return;
  pickup_visible_map = false;
  pickup_visible_dash = true;
  pickup_x = px;
  pickup_y = py;
}
//Called to hide any visible pickups that havent been collected
void pickup_entered_room( void ){
  pickup_visible_map = false;
  pickup_visible_dash = false;
}
//Called to draw any pickups visible in the room
char pickup_draw_oam( char oam_id ){
  char base_index;
  if ( pickup_visible_map )
    base_index = 0xC4;
  else if ( pickup_visible_dash )
    base_index = 0xE4;
  else //huh?
    return oam_id;
  //Draw the pickup
  oam_id = oam_spr( pickup_x, pickup_y, base_index, 0x02, oam_id );
  oam_id = oam_spr( pickup_x, pickup_y+8, base_index+1, 0x02, oam_id );
  oam_id = oam_spr( pickup_x+8, pickup_y, base_index+2, 0x02, oam_id );
  oam_id = oam_spr( pickup_x+8, pickup_y+8, base_index+3, 0x02, oam_id );
  return oam_id;
}

void pickup_collision_check( void ){
  if ( pickup_visible_map ){
    if ( player_pos_x[0] + 8 >= pickup_x 
        && player_pos_x[0] - 8 <= pickup_x 
        && player_pos_y[0] + 8 >= pickup_y
        && player_pos_y[0] - 8 <= pickup_y
    ){
      pickup_visible_map = false;
      pickup_collected_map = true;
    }
  }
  if ( pickup_visible_dash ){
    if ( player_pos_x[0] + 8 >= pickup_x 
        && player_pos_x[0] - 8 <= pickup_x 
        && player_pos_y[0] + 8 >= pickup_y
        && player_pos_y[0] - 8 <= pickup_y
    ){
      pickup_visible_dash = false;
      pickup_collected_dash = true;
    }
  }
}