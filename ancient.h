#ifndef ANCIENT_H
#define ANCIENT_H

#define DIR_NORTH 0
#define DIR_EAST 1
#define DIR_SOUTH 2
#define DIR_WEST 3

extern char ancient_is_animation_frame, ancient_frame_count;
void ancient_player_left_room( char direction );
void ancient_player_dies( void );
void ancient_player_spawn_at_spawn_point( void );
#endif