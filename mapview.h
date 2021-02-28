#ifndef MAPVIEW_H
#define MAPVIEW_H

#define MAPVIEW_MAP_WIDTH 15
#define MAPVIEW_MAP_HEIGHT 13
#define MAPVIEW_DATA_OFFSET_X 1
#define MAPVIEW_DATA_OFFSET_Y 1

#define MAPVIEW_TILE_EMPTY 0
#define MAPVIEW_TILE_SEEN 1
#define MAPVIEW_TILE_EXPLORED 2
#define MAPVIEW_TILE_PLAYER 3

extern char mapview_mapdata[];

void mapview_reset( void );
void mapview_cheat( void );
void mapview( void ); //switch to mapview mode
void mapview_entered_room( void ); //notify mapview that a room was entered, let it update the map

#endif
