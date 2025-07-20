#include "gameloop.h"
#include "skyelib.h"
#include <SDL.h>

/*
init
-- the start of the game loop ~~~ 
-- Called ONCE when the game starts!!
*/
void init()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    DisableCursor(); // Limit cursor to relative movement inside the window
    SetTargetFPS(FPS);

    printf("\n \n ### SKYESRC ### \n \n");
    console_init();

    char raylib_version_string[64];
    snprintf(raylib_version_string, sizeof(raylib_version_string),
         "Raylib: v%d.%d.%d", 
         RAYLIB_VERSION_MAJOR, 
         RAYLIB_VERSION_MINOR, 
         RAYLIB_VERSION_PATCH);

    SDL_version compiled;
    SDL_VERSION(&compiled);  // Get SDL version the program was compiled against
    char sdl_version_str[64];
    snprintf(sdl_version_str, sizeof(sdl_version_str), "SDL: v%d.%d.%d", compiled.major, compiled.minor, compiled.patch);

    int glsl_major = GLSL_VERSION / 100;
    int glsl_minor = (GLSL_VERSION / 10) % 10;
    int glsl_patch = GLSL_VERSION % 10;
    char glsl_version_str[64];
    snprintf(glsl_version_str, sizeof(glsl_version_str), "GLSL: v%d.%d.%d\n", glsl_major, glsl_minor, glsl_patch);

    console_log(raylib_version_string);
    console_log(sdl_version_str);
    console_log(rlGetVersionString());
    console_log(glsl_version_str);

    console_line();

    console_log("Initializing engine...");
    console_log("Creating camera...");
    camera_init();
    console_log("Adding lights to level...");
    lights_init();
    map_parse("test.map");
}