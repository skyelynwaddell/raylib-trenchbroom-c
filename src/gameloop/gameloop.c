#define RAYGUI_IMPLEMENTATION
#define RAYLIB_RAYCOLLISION_IMPLEMENTATION

#include "skyelib.h"
#include "gameloop.h"
#include "player.h"
#include "enemy.h"
#include "global.h"
#include <OpenGL/gl3.h>


static Camera weaponCamera = {
    .position = {0.0f, 0.0f, 0.0f},
    .target = {0.0f, 0.0f, -1.0f},   // Looking forward (negative Z)
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 90.0f,
    .projection = CAMERA_PERSPECTIVE
};
static Shader viewmodel_shader;

static void BeginModeViewModel()
{
    rlDrawRenderBatchActive();
    glClear(GL_DEPTH_BUFFER_BIT);
    BeginShaderMode(sh_light);
    BeginMode3D(weaponCamera);
}

static void EndModeViewModel()
{
    EndShaderMode();
    EndMode3D();
}

// Program Entry Point
// -----------------------------
int gameloop()
{
    // Initialization
    // -----------------------------
    global_game_loading = true;
    global_paused = true;
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4X

    init();
    player_init();
    GuiLoadStyle(STYLE_SKYE);

    viewmodel_init();

    Vector3 pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 10};
    enemy_create(ENEMY_SHOTGUNNER, pos);

    pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 15};
    Enemy *en2 = enemy_create(ENEMY_SHOTGUNNER, pos);
    en2->model.current_anim = ANIM_SHOTGUNNER_RUN;

    global_paused = false;
    global_game_loading = false;

    console_line();

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
            BeginShaderMode(sh_light);
            BeginMode3D(camera);
                draw();
                player_draw();
                enemy_draw_all();
            EndMode3D();
            EndShaderMode();

            // View Model
            // -----------------------------
            BeginModeViewModel();
                viewmodel_draw(&weaponCamera);
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