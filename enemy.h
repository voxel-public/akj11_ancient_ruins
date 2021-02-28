#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_WAYPOINT_COUNT 6

extern char enemy_is_active; //is the enemy active in the current room?
extern char enemy_pos_x[];
extern char enemy_pos_y[];
extern char enemy_waypoints_x[ENEMY_WAYPOINT_COUNT];
extern char enemy_waypoints_y[ENEMY_WAYPOINT_COUNT];

void enemy_deactivate( void );
void enemy_activate( void ); //Set enemy_waypoints_x and _y before calling
void enemy_tick( void ); //Move the enemy
char enemy_draw_oam( char oam_id ); //Draw the enemy to screen

#endif
