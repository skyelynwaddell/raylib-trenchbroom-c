#include "skyelib.h"

/*
main
Entry point of the program.
Initializes the window, ENet server, and starts the main loop.
*/
int main(void) {

    enet_initialize();
    atexit(enet_deinitialize);
    InitWindow(600, 600, "skyesrc server");

    while (!WindowShouldClose()) {
        // Poll network events

        if (server_online)
            enetserver_update();

        // Draw server status and messages
        enetserver_draw_gui();
    }

    enet_host_destroy(server);
    CloseWindow();

    return false;
}
