#ifndef PICKUPS_H
#define PICKUPS_H

extern char pickup_collected_map;
extern char pickup_collected_dash;
extern char pickup_visible_map;
extern char pickup_visible_dash;
extern char pickup_x, pickup_y; //Position of onscreen pickup (only one is ever visible)

//Called when entering a room with a pickup to start displaying it
void pickup_entered_map_room( char px, char py );
void pickup_entered_dash_room( char px, char py );
//Called to hide any visible pickups that havent been collected
void pickup_entered_room( void );
//Called to draw any pickups visible in the room
char pickup_draw_oam( char oam_id );
void pickup_collision_check( void );

#endif
