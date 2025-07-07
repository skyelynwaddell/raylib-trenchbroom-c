#include <stdio.h>
#include "raylib.h"
#include "raygui.h"

#include "./headers/defs.h"
#include "./headers/global.h"
#include "./headers/camera.h"
#include "./headers/map.h"
#include "./headers/cleanup.h"

#include "./headers/init.h"
#include "./headers/input.h"
#include "./headers/update.h"
#include "./headers/draw.h"
#include "./headers/draw_viewmodel.h"
#include "./headers/draw_gui.h"
#include "./headers/player.h"

#include "./lib/raylib/include/rcamera.h"

// Program Entry Point
// -----------------------------
int main()
{
    // Initialization
    // -----------------------------
    Player player;
    init();
    player_init(&player);
    GuiLoadStyle(STYLE_AMBER); // Load GUI style from file

    // Main Game Loop
    while(!WindowShouldClose())
    {
        SetExitKey(0); // Disable exit key (ESC)
        input();
        update();
        player_update(&player);

        // Draw
        // -----------------------------
        BeginDrawing();

            draw();
            draw_viewmodel();
            EndMode3D();
            draw_gui();

        EndDrawing();
        // -----------------------------
    }

    // De-Initialization
    // -----------------------------
    
    clean_up();
    CloseWindow();
    return 0;
    // -----------------------------
}