#ifndef DEFS_H
#define DEFS_H

/*
Toggles
These defines can be toggled to change the games core logic
*/
#define PLATFORM_DESKTOP
// -------------


/*
Constants
These values never change
*/

// game settings
#define GAME_TITLE "skyesrc"
#define FPS 2000 // max fps
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// brush & map geometry defs
#define BRUSH_FACE_COUNT 64 // a brush can have up to 64 faces - must be convex
#define MAX_COLUMNS 20
#define MAX_TEXTURES 10000
#define MAX_POLY_VERTS 64
#define MAX_LINE 1024
#define MAX_BRUSHES 10000
#define MAX_POLYGONS 10000
#define MAX_VERTICES_PER_FACE 128

// level settings
#define MAX_ENTITIES 1000 // Maximum entities can be in a room
#define MAX_LIGHTS 255 // Maximum lightobjects can be in a room
#define PLAYER_SPAWN_GAP 20 // how high to increase the player spawn gap so isnt stuck in floor
#define MAX_DARK 0.2 // How dark the room can get without lighting (0 = BLACK)


// raygui & styles/themes
#define RAYGUI_IMPLEMENTATION

#define STYLE_PATH "styles/"
#define STYLE_AMBER STYLE_PATH "style_amber.rgs"
#define STYLE_ASHES STYLE_PATH "style_ashes.rgs"
#define STYLE_BLUISH STYLE_PATH "style_bluish.rgs"
#define STYLE_CANDY STYLE_PATH "style_candy.rgs"
#define STYLE_CHERRY STYLE_PATH "style_cherry.rgs"
#define STYLE_CYBER STYLE_PATH "style_cyber.rgs"
#define STYLE_DARK STYLE_PATH "style_dark.rgs"
#define STYLE_ENFETE STYLE_PATH "style_enfete.rgs"
#define STYLE_GENESIS STYLE_PATH "style_genesis.rgs"
#define STYLE_JUNGLE STYLE_PATH "style_jungle.rgs"
#define STYLE_LAVANDA STYLE_PATH "style_lavanda.rgs"
#define STYLE_RLTECH STYLE_PATH "style_rltech.rgs"
#define STYLE_SUNNY STYLE_PATH "style_sunny.rgs"
#define STYLE_TERMINAL STYLE_PATH "style_terminal.rgs"


#endif