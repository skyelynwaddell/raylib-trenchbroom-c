#define RAYGUI_IMPLEMENTATION
#define RAYLIB_RAYCOLLISION_IMPLEMENTATION

#include "skyelib.h"
#include "gameloop.h"
#include "player.h"
#include "enemy.h"
#include "global.h"
#include <OpenGL/gl3.h>

// Program Entry Point
// -----------------------------
int gameloop()
{
    // Initialization
    // -----------------------------
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4X
    GuiLoadStyle(STYLE_SKYE);
    init();
    Vector3 pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 10};
    enemy_create(ENEMY_SHOTGUNNER, pos);

    pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 15};
    Enemy *en2 = enemy_create(ENEMY_SHOTGUNNER, pos);
    en2->model.current_anim = ANIM_SHOTGUNNER_RUN;
    // Main Game Loop
    // -----------------------------
    while(global_quit_game == false && !WindowShouldClose())
    {
        if (global_game_loading == false)
        {
        SetExitKey(false); // Disable exit key (ESC)
        input();
        update();
        player_update();
        enemy_update_all();
        lights_update();

        // Draw
        // -----------------------------
        BeginDrawing();
            ClearBackground(BLACK);

            // 3D World
            // -----------------------------
            BeginMode3D(camera);
            BeginShaderMode(sh_light);

                draw();
                player_draw();
                enemy_draw_all();

            EndShaderMode();
            EndMode3D();

            // View Model
            // -----------------------------
            BeginModeViewModel();
            BeginShaderMode(sh_light);

                viewmodel_draw();

            EndShaderMode();
            EndModeViewModel();

            draw_gui();

        EndDrawing();
        // draw -------------------------

        global_player_shooting = false;
        global_raycast_has_target = false;
        }
    }

    // De-Initialization
    // -----------------------------
    clean_up();
    UnloadShader(sh_light);
    enemy_cleanup_all();
    texture_cache_cleanup();
    CloseWindow();
    return false;
    // de-init ----------------------
}


void BeginModeViewModel()
{
    rlDrawRenderBatchActive();
    glClear(GL_DEPTH_BUFFER_BIT);
    BeginMode3D(viewmodel.camera);
}

void EndModeViewModel()
{
    EndMode3D();
}