#include "skyelib.h"

/*
main
Entry point of the program.
Initializes the window, ENet server, and starts the main loop.
*/
int main(void) {

    enet_initialize();
    atexit(enet_deinitialize);

    #ifndef SERVER_HEADLESS
        //SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(420, 360, "skyesrc server");
    #else
        enetserver_start();
    #endif

    // Poll network events
    #ifndef SERVER_HEADLESS
        while (!WindowShouldClose()) {

            if (server_online)
                enetserver_update();

            // Draw server status and messages
            BeginDrawing();
                ClearBackground(DARKGRAY);
                enetserver_draw_gui();
            EndDrawing();
        }
    #else
        while (server_online) 
        {
            enetserver_update();
        }
    #endif

    enet_host_destroy(server);

    #ifndef SERVER_HEADLESS
        CloseWindow();
    #endif

    return false;
}
