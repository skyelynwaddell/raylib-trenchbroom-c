#include "skyelib.h"

/*
main
Entry point of the program.
Initializes the window, ENet server, and starts the main loop.
*/
int main(void) {

    enet_initialize();
    atexit(enet_deinitialize);

    parse_server_properties("server.properties");
    printf("Server IP: %s\n", SERVER_IP);
    printf("Server Port: %d\n", SERVER_PORT);

    if (SERVER_HEADLESS == false)
    {
        //SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(420, 360, "skyesrc server");
    }
    else
    {
        enetserver_start();
    }

    // Poll network events
    if (SERVER_HEADLESS == false){
        while (!WindowShouldClose()) {

            if (server_online)
                enetserver_update();

            // Draw server status and messages
            BeginDrawing();
                ClearBackground(DARKGRAY);
                enetserver_draw_gui();
            EndDrawing();
        }
    }
    else
    {
        while (server_online) 
        {
            enetserver_update();
        }
    }

    enet_host_destroy(server);

    if (SERVER_HEADLESS == false)
        CloseWindow();

    return false;
}
