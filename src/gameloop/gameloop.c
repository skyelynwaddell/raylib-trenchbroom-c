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


    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    
    // --- Test Monsters ---
    Vector3 pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 10};
    enemy_create(ENEMY_SHOTGUNNER, pos);

    pos = (Vector3){global_player_spawn.x - 10, global_player_spawn.y, global_player_spawn.z - 15};
    Enemy *en2 = enemy_create(ENEMY_SHOTGUNNER, pos);
    en2->model.current_anim = ANIM_SHOTGUNNER_RUN;


    int server_online = true;
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

        if (global_game_loading == false)
        {
            input();
            update();

            // Update virtual mouse (clamped mouse value behind game screen)
            Vector2 mouse = GetMousePosition();
            Vector2 virtualMouse = { 0 };
            float scale = Min((float)GetScreenWidth()/GAME_SCREEN_WIDTH, (float)GetScreenHeight()/GAME_SCREEN_HEIGHT);

            virtualMouse.x = (mouse.x - (GetScreenWidth() - (GAME_SCREEN_WIDTH*scale))*0.5f)/scale;
            virtualMouse.y = (mouse.y - (GetScreenHeight() - (GAME_SCREEN_HEIGHT*scale))*0.5f)/scale;
            virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)GAME_SCREEN_WIDTH, (float)GAME_SCREEN_HEIGHT });

            // Apply the same transformation as the virtual mouse to the real mouse (i.e. to work with raygui)
            SetMouseOffset(-(GetScreenWidth() - (GAME_SCREEN_WIDTH*scale))*0.5f, -(GetScreenHeight() - (GAME_SCREEN_HEIGHT*scale))*0.5f);
            SetMouseScale(1/scale, 1/scale);

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
                           (Rectangle){ (GetScreenWidth() - ((float)GAME_SCREEN_WIDTH*scale))*0.5f, (GetScreenHeight() - ((float)GAME_SCREEN_HEIGHT*scale))*0.5f,
                           (float)GAME_SCREEN_WIDTH*scale, (float)GAME_SCREEN_HEIGHT*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);

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