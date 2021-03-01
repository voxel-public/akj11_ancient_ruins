#include "enemy.h"
#include "neslib.h"
#include "room.h"
#include "ancient.h"
#include "player.h"

#define ENEMY_SPEED_PATROL 1
#define ENEMY_SPEED_ATTACK 4
#define ENEMY_POS_COUNT 20
#define ENEMY_SEGMENT_COUNT 5

#define ENEMY_LEAD_X_NEGATIVE -4
#define ENEMY_LEAD_X_POSITIVE 0
#define ENEMY_LEAD_Y_NEGATIVE -1
#define ENEMY_LEAD_Y_POSITIVE 0
#define ENEMY_TILE_BASE 0x80

char enemy_is_active; //is the enemy active in the current room?
char enemy_pos_x[ENEMY_POS_COUNT];
char enemy_pos_y[ENEMY_POS_COUNT];
char enemy_directions[ENEMY_POS_COUNT];
char enemy_waypoints_x[ENEMY_WAYPOINT_COUNT];
char enemy_waypoints_y[ENEMY_WAYPOINT_COUNT];
char enemy_target_waypoint;
char enemy_dest_x, enemy_dest_y;
char enemy_prev_map_x, enemy_prev_map_y;

void enemy_deactivate(){
  
  enemy_is_active = false;
  
}
void enemy_activate(){
  char i = 0;
  //Allow enemy position to be preserved across map views
  if ( enemy_prev_map_x != player_map_x || 
      enemy_prev_map_y != player_map_y )
  {
    enemy_target_waypoint = 0;  
    while( i < ENEMY_POS_COUNT ){
      enemy_pos_x[i] = enemy_waypoints_x[5];
      enemy_pos_y[i] = enemy_waypoints_y[5];
      ++i;
    }
  }
  enemy_is_active = true;
  enemy_prev_map_x = player_map_x;
  enemy_prev_map_y = player_map_y;
}


//Normal tick to perform when the player is not in the water
void enemy_tick_patrol( void ){
  //Try and get unstuck 
  if ( enemy_pos_x[3] == enemy_pos_x[0] && enemy_pos_y[3] == enemy_pos_y[0] ){
    enemy_target_waypoint = enemy_target_waypoint + 1 ;
  }
  if ( enemy_pos_x[0] == enemy_dest_x && enemy_pos_y[0] == enemy_dest_y ){
    enemy_target_waypoint = enemy_target_waypoint + 1 ;
  }
  if ( enemy_target_waypoint >= ENEMY_WAYPOINT_COUNT )
    enemy_target_waypoint = 0;
  enemy_dest_x = enemy_waypoints_x[enemy_target_waypoint];
  enemy_dest_y = enemy_waypoints_y[enemy_target_waypoint]; 
}
//Player is in the water, try and eat them
void enemy_tick_attack( void ){
  
  //try and get unstuck
  if ( enemy_pos_x[3] == enemy_pos_x[0] && enemy_pos_y[3] == enemy_pos_y[0] ){
    enemy_dest_x = enemy_dest_x + ancient_frame_count;
    enemy_dest_y = enemy_dest_y - ancient_frame_count;
  } else {
    enemy_dest_x = player_pos_x[0];
    enemy_dest_y = player_pos_y[0]; 
  }
  //eat
  if ( enemy_pos_x[0] + 6 > player_pos_x[0] && enemy_pos_x[0] - 6 < player_pos_x[0] &&
      enemy_pos_y[0] + 6 > player_pos_y[0] && enemy_pos_y[0] - 6 < player_pos_y[0] ){
    ancient_player_dies();
  }
}

void enemy_tick( void ){
  char lead_x, lead_y, future_x, future_y;
  signed char dx, dy;
  char i;
  char ticks = 1;
  if ( !enemy_is_active )
    return;
  //If the player is in the water, gotta go fast
  if ( player_state & PLAYER_STATE_SUBMERGED ){
    ticks = 3;
  }
  i = ENEMY_POS_COUNT-1;
  //Cycle the enemy positions for animation
  while ( i > 0 ){
    enemy_pos_x[i] = enemy_pos_x[i-1];
    enemy_pos_y[i] = enemy_pos_y[i-1];
    enemy_directions[i] = enemy_directions[i-1];
    --i; 
  }
  //Rerun the entire enemy logic * ticks because it's easier than letting the enemy move
  //multiple pixels per step
  for ( i = 0; i < ticks; ++i ){
    if ( player_state & PLAYER_STATE_SUBMERGED || player_state & PLAYER_STATE_DEAD ){
      enemy_tick_attack();
    } else {
      enemy_tick_patrol(); 
    }
  
    dx = 0;
    dy = 0;
    lead_x = 0;
    lead_y = 0;

    if ( enemy_dest_x > enemy_pos_x[0] ){
      dx = 1;
      lead_x = ENEMY_LEAD_X_POSITIVE;
    } else if ( enemy_dest_x < enemy_pos_x[0] ){
      dx = -1;
      lead_x = ENEMY_LEAD_X_NEGATIVE;
    } 
    if ( enemy_dest_y > enemy_pos_y[0] ){
      dy = 1;
      lead_y = ENEMY_LEAD_Y_POSITIVE;
    } else if ( enemy_dest_y < enemy_pos_y[0] ){
      dy = -1;
      lead_y = ENEMY_LEAD_Y_NEGATIVE;
    }
    //Enemy has directions
    //Set the head to the appropriate direction for this movement
    if ( dx == 0 && dy < 0 ) enemy_directions[0] = 0;
    if ( dx > 0 && dy < 0 ) enemy_directions[0] = 1;
    if ( dx > 0 && dy == 0 ) enemy_directions[0] = 2;
    if ( dx > 0 && dy > 0 ) enemy_directions[0] = 3;
    if ( dx == 0 && dy > 0 ) enemy_directions[0] = 4;
    if ( dx < 0 && dy > 0 ) enemy_directions[0] = 5;
    if ( dx < 0 && dy == 0 ) enemy_directions[0] = 6;
    if ( dx < 0 && dy < 0 ) enemy_directions[0] = 7;

    future_x = enemy_pos_x[0] + dx;
    future_y = enemy_pos_y[0] + dy;
    if ( room_is_square_wet( enemy_pos_x[0]-2, future_y-2, 4 ) ){
      enemy_pos_y[0] = future_y;
    }
    if ( room_is_square_wet( future_x-2, enemy_pos_y[0]-2, 4 ) ){
      enemy_pos_x[0] = future_x;
    }
  }
  
}

char enemy_draw_oam( char oam_id ){
  char sprite, index, dir, attr;
  char i = 0;
  if ( !enemy_is_active )
    return oam_id;
  while( i < ENEMY_SEGMENT_COUNT ){
    attr = 0x03;
    index = i << 2;
    dir = enemy_directions[index];
    if ( dir == 0 ){ sprite = ENEMY_TILE_BASE; }
    else if ( dir == 1 ){ sprite = ENEMY_TILE_BASE + 1; }
    else if ( dir == 2 ){ sprite = ENEMY_TILE_BASE + 2; }
    else if ( dir == 3 ){ sprite = ENEMY_TILE_BASE + 1; attr |= OAM_FLIP_V; }
    else if ( dir == 4 ){ sprite = ENEMY_TILE_BASE; }
    else if ( dir == 5 ){ sprite = ENEMY_TILE_BASE + 1; }
    else if ( dir == 6 ){ sprite = ENEMY_TILE_BASE + 2; }
    else if ( dir == 7 ){ sprite = ENEMY_TILE_BASE + 1; attr |= OAM_FLIP_H; }
    oam_id = oam_spr( enemy_pos_x[index], enemy_pos_y[index], sprite, attr, oam_id );
    ++i;
  }
  
  return oam_id;
}