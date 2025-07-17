#define RAYGUI_IMPLEMENTATION

#include "gameloop.h"
#include "map.h"
#include "player.h"


// Program Entry Point
// -----------------------------
int gameloop()
{
    // Initialization
    // -----------------------------
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4X

    init();
    player_init();
    GuiLoadStyle(STYLE_TERMINAL);

    // Main Game Loop
    // -----------------------------
    while(!WindowShouldClose())
    {
        SetExitKey(false); // Disable exit key (ESC)
        input();
        update();
        player_update();
        lights_update();

        // Draw
        // -----------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            BeginMode3D(camera);
            BeginShaderMode(sh_light);
                draw();
                player_draw();
                draw_viewmodel(); 
            EndShaderMode();
            EndMode3D();

            draw_gui();

        EndDrawing();
        // draw -------------------------

    }

    // De-Initialization
    // -----------------------------
    clean_up();
    UnloadShader(sh_light);
    CloseWindow();
    return false;
    // de-init ----------------------
}