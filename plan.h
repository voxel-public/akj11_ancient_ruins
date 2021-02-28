/*

Alakajam #11 48 hour gamejam entry for theme ANCIENT RUINS, made in 8bitworkshop IDE

0.0	Project setup
*0.0.1	Project save/load test - make sure it's possible to resume work later if the browser is closed
*0.0.2	Game idea and rest of plan

0.1	Single room prototype
*0.1.1	Draw a test room to screen
*0.1.2	Draw a player sprite to screen
*0.1.3	Player can be moved
*0.1.4	Player to wall collision detection and handling
*0.1.5	Player can leave screen (doesn't do anything, but room exit is triggered, maybe teleport player)

0.2	Multi room prototype
*0.2.1	Define a few test rooms
*0.2.2	Define a meta map of rooms
*0.2.3	Allow player to move between rooms

0.3 	Animated player
*0.3.1	Static player sprite
*0.3.2	Animation
*0.3.3	Directional animated
*0.3.4	Stateful (keep last direction moved when stationary)

0.4	Player statuses
*0.4.1	Submerge
*0.4.2	Death
*0.4.3	Reforming

0.5	Map
*0.5.1	Map designed
*0.5.2	Map drawn in map editor
*0.5.3	Map ingame
	Convert room data to bytecode
        Layout map in ROM
*0.5.4	Map collectible art
*0.5.5	Map interface art
*0.5.6	Track room status
		Unknown
                Explored
                Adjacent (been in a neighbouring room)
*0.5.7	Render map
*0.5.8	Don't render map pickup if already collected

0.6 	Dash
*0.6.1	Dash collectible art
*0.6.2	Dash effect art
*0.6.3	Dash effect
*0.6.4	Don't render dash pickup if already collected

0.7 	Enemy
*0.7.1	Enemy prototype
*0.7.2	Kill player on contact
*0.7.3	Enemy patrol pathfinding 
*0.7.4 	Enemy seek behaviour
*0.7.5	Enemy polish

0.8 	Other map elements
*0.8.1	Tile art
	Barrier A active, inactive
        Barrier B active, inactive
        Directional tiles
        Slider tiles
        Toggle switches
        Spawn point
*0.8.2	Barrier state
*0.8.3  Barrier toggle
*0.8.4 	Spawn point
*0.8.5	Pushtiles enforce movement
*0.8.6 	Pushtiles disable dash

0.9	Audio
*0.9.1	General music (terrible!)
*0.9.2	Danger music (also bad but less frequent!)
*0.9.3	Dash effect
*0.9.4 	Pickup effect
*0.9.5 	Barrier switch
*0.9.6	Die
*0.9.7	Reform

0.10	Interest
0.10.1	Critters?
0.10.2	More interesting map tiles

Game Idea: ADVENTURELIKE

Explore a simple, abstract, ruined world with few hazards. Collect a few key items to unlock areas and make 
escape possible. Death is punished by returning the player to the nearest activated spawn point, of which
there are many. 

Some hazards/puzzles:

An area that can only be passed by missing a spawn point, intentionally dying and respawning at a previous
spawn point.

An underwater danger that must be distracted 

Globally toggleable walls, standard switches throughout the world allow these walls to be lowered or raised

Sliding ice / directional movement tiles like chips challenge or pokemon

Add a cherry stem?
*/