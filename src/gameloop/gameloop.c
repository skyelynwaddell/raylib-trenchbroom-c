#include "skyelib.h"
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
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4X
    init();

    // --- Test Monsters ---
    Vector3 pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 10};
    enemy_create(ENEMY_SHOTGUNNER, pos);

    pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 15};
    Enemy *en2 = enemy_create(ENEMY_SHOTGUNNER, pos);
    en2->model.current_anim = ANIM_SHOTGUNNER_RUN;


    // --- ENET Client Initialization ---
    if (enetclient_init() == false) {
        printf("Failed to initialize ENet client.\n");
    }

    // Main Game Loop
    // -----------------------------
    while(global_quit_game == false && !WindowShouldClose())
    {
        enetclient_update();

        if (global_game_loading == false)
        {
            input();
            update();

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
                BeginShaderMode(sh_viewmodel);

                    viewmodel_draw();

                EndShaderMode();
                EndModeViewModel();

                draw_gui();

            EndDrawing();
        }
    }

    // De-Initialization
    // -----------------------------
    clean_up();

    CloseWindow();
    enetclient_disconnect();
    return false;
    // de-init ----------------------
}