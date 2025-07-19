#define RAYGUI_IMPLEMENTATION
#define RAYLIB_RAYCOLLISION_IMPLEMENTATION

#include "gameloop.h"
#include "player.h"
#include "enemy.h"
#include "global.h"


// Program Entry Point
// -----------------------------
int gameloop()
{
    // Initialization
    // -----------------------------
    global_game_loading = true;
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4X

    init();
    player_init();
    GuiLoadStyle(STYLE_TERMINAL);

    Vector3 pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 10};
    enemy_create(ENEMY_SHOTGUNNER, pos);

    pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 15};
    Enemy *en2 = enemy_create(ENEMY_SHOTGUNNER, pos);
    en2->model.current_anim = ANIM_SHOTGUNNER_RUN;

    // Main Game Loop
    // -----------------------------
    while(global_quit_game == false && !WindowShouldClose())
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
            BeginMode3D(camera);
            BeginShaderMode(sh_light);
                draw();
                player_draw();
                enemy_draw_all();
                draw_viewmodel();

            EndShaderMode();
            EndMode3D();
            draw_gui();

        EndDrawing();

        global_player_shooting = false;
        global_raycast_has_target = false;
        // draw -------------------------

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