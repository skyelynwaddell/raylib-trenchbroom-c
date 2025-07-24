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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4X
    
    init();

    SetWindowMinSize(320, 240);
    set_screen_size(SCREEN_SIZE_1280);
    font_set_size((int)(FONT_SIZE_DEFAULT * gui_scale));

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use
    update_virtual_mouse();

    // --- Test Monsters ---
    Vector3 pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 10};
    enemy_create(ENEMY_SHOTGUNNER, pos);

    pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 15};
    Enemy *en2 = enemy_create(ENEMY_SHOTGUNNER, pos);
    en2->model.current_anim = ANIM_SHOTGUNNER_RUN;


    int server_online = false;
    // --- ENET Client Initialization ---
    if (server_online)
    {
        if (enetclient_init() == false) 
        {
            printf("Failed to initialize ENet client.\n");
            server_online = false;
        }
    }

    // Main Game Loop
    // -----------------------------
    while(global_quit_game == false && !WindowShouldClose())
    {
        if (server_online)
            enetclient_update();

        if (IsKeyPressed(KEY_FIVE)) set_screen_size(SCREEN_SIZE_640);
        if (IsKeyPressed(KEY_SIX)) set_screen_size(SCREEN_SIZE_1280);
        if (IsKeyPressed(KEY_SEVEN)) set_screen_size(SCREEN_SIZE_1920);

        if (IsWindowResized())
        {
            target = LoadRenderTexture(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
            update_virtual_mouse(); 
        }

        if (global_game_loading == false)
        {
            input();
            update();


            // Draw
            // -----------------------------
            BeginTextureMode(target);
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

            EndTextureMode();

            BeginDrawing();

                ClearBackground(BLACK);
                DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)GAME_SCREEN_WIDTH*window_scale))*0.5f, (GetScreenHeight() - ((float)GAME_SCREEN_HEIGHT*window_scale))*0.5f,
                           (float)GAME_SCREEN_WIDTH*window_scale, (float)GAME_SCREEN_HEIGHT*window_scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
            
            EndDrawing();
        }
    }

    // De-Initialization
    // -----------------------------
    clean_up();

    UnloadRenderTexture(target);

    CloseWindow();
    enetclient_disconnect();
    return false;
    // de-init ----------------------
}