#include "skyelib.h"

ENetHost *server;
ENetAddress address;
ENetEvent event;
MultiplayerUser users[MAX_PLAYERS];
int users_count = 0;
int server_online = false;

/*
enetserver_init
Initializes the ENet server and starts listening on the specified port.
*/
int enetserver_init()
{
    // Initialize ENet
    if (enet_initialize() != 0) 
    {
        fprintf(stderr, "ENet initialization failed.\n");
        CloseWindow();
        return false;
    }
    atexit(enet_deinitialize);

    address.host = ENET_HOST_ANY;
    address.port = SERVER_PORT;
    server = enet_host_create(&address, 32, 2, 0, 0);

    if (!server) 
    {
        fprintf(stderr, "Failed to create ENet server host.\n");
        CloseWindow();
        return false;
    }

    printf("🟢 Server Online! %x:%u\n", address.host, address.port);
    return true;
}


/*
enetserver_disconnect_user
Disconnects a user from the server and removes them from the user list.
*/
static void enetserver_disconnect_user(ENetPeer *peer)
{
    for (int i = 0; i < users_count; i++) 
    {
        if (users[i].peer == event.peer) 
        {
            printf("Player %d (%s) disconnected.\n", users[i].player_id, users[i].username);
            
            // Remove user from the list
            for (int j = i; j < users_count - 1; j++) 
            {
                users[j] = users[j + 1];
            }
            users_count--;
            break;
        }
    }
    event.peer->data = NULL;
}


/*
enetserver_process_message
Processes incoming messages from clients.
*/
void enetserver_process_message(ENetEvent *event)
{
    ENetPacket *packet = event->packet;

    if (packet->dataLength < 1)
    {
        printf("Received empty packet.\n");
        return;
    }

    uint8_t message_type = packet->data[0]; // First byte is the message type
    unsigned char *payload = packet->data + 1;
    size_t payload_len = packet->dataLength - 1;

    if (payload_len < 0)
    {
        printf("Received invalid HELLO message length: %zu\n", payload_len);
        return;
    }

    switch (message_type) {

        case MESSAGE_TYPE_CONNECT:
            if (payload_len < sizeof(MultiplayerUser)) 
            {
                printf("Received CONNECT message with invalid length: %zu\n", payload_len);
                return;
            }

            MultiplayerUser user = {0};
            memcpy(&user, payload, sizeof(MultiplayerUser));

            int player_id = users_count++; 
            user.player_id = player_id; 
            user.peer = event->peer; // Store the peer in the user struct
            event->peer->data = &user; // Store user data in the peer for later reference

            // Process the user connection
            users[player_id] = user;
            printf("Player connected: ID=%d, Username=%s \n", player_id, user.username);
        break;

        default:
            printf("Unknown message type: %d\n", message_type);
        break;
    }
}


/*
enetserver_update
Polls for incoming messages from clients and handles them.
This function should be called in the main game loop to keep the server updated.
*/
void enetserver_update()
{
    // Poll ENet events non-blocking (timeout 0)
    while (enet_host_service(server, &event, 0) > 0) {
        switch (event.type) {

            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                        event.peer->address.host,
                        event.peer->address.port);
            break;

            case ENET_EVENT_TYPE_RECEIVE:
                enetserver_process_message(&event);
                enet_packet_destroy(event.packet);

            break;

            case ENET_EVENT_TYPE_DISCONNECT:
                enetserver_disconnect_user(event.peer);
            break;

            default: break;
        }
    }
}


/*
enetserver_start
Starts the ENet server and begins listening for incoming connections.
Called when user presses "Start Server" button.
*/
void enetserver_start()
{
    // start server
    if (enetserver_init()) 
    {
        server_online = true;
        return;
    }
    printf("Failed to start server. Is another server already running?\n");
}


/*
enetserver_stop
Stops the ENet server and cleans up resources.
Called when user presses "Stop Server" button.
*/
void enetserver_stop()
{
    // stop server
    if (server != NULL) 
    {
        enet_host_destroy(server);
        server = NULL;
    }
    users_count = 0;
    memset(users, 0, sizeof(users));
    server_online = false;
    printf("Server stopped.\n");
}


// --- Draw Variabless ---
static int x = 10;
static int y = 10;
    // Pause Menu Title
static int line_gap = 20;
static void newline(){ y+=line_gap; }
static int g = 10;  // gap 
static int fs = 20; // font size
/*
draw
Draws the server status and messages on the screen.
*/
void enetserver_draw_gui()
{
    x = g;
    y = g;

    int btn_h = 50;
    int btn_w = 200;

    DrawText("Skyesrc Server", x, y, fs, LIGHTGRAY);

    font_set_size(20);
    if (GuiButton(
        (Rectangle){ g, GetScreenHeight() - btn_h - g, btn_w, btn_h }, 
        GuiIconText(server_online ? ICON_PLAYER_STOP : ICON_PLAYER_RECORD, 
                    server_online ? "Stop Server" : "Start Server"
    ))) 
    {
        server_online ? enetserver_stop() : enetserver_start();
    } newline();

    if (server_online == false)
    {
        DrawText("Server Offline", x, y, fs, RED); newline();
        return;
    }

    DrawText("Server Online", x, y, fs, GREEN); newline();
    DrawText(TextFormat("Port: %u\n", address.port), x, y, fs, LIGHTGRAY); newline();
    DrawText(TextFormat("Connected players: %d", users_count), x, y, fs, LIGHTGRAY); newline();

    // --- Scrollable user grid ---
    static Vector2 scroll = { 0, 0 }; // static to preserve scroll position
    Rectangle panelBounds = { x, y, 400, 130 }; // Panel position and size
    int gridCols = 1; // Change to 2+ for multi-column
    int cellW = 380;  // Make cell width fit panel
    int cellH = 28;
    Rectangle contentBounds = { 0, 0, cellW, users_count * cellH }; // Content size

    Rectangle view = {0};
    GuiScrollPanel(panelBounds, NULL, contentBounds, &scroll, &view);

    // Draw grid cells inside the scroll panel
    for (int i = 0; i < users_count; i++) {
        int col = i % gridCols;
        int row = i / gridCols;
        int cellX = view.x + col * cellW;
        int cellY = view.y + row * cellH + (int)scroll.y;

        Rectangle cellRect = { cellX, cellY, cellW - 20, cellH };

        // Only draw if cell is inside the visible view
        if (CheckCollisionRecs(cellRect, view)) {
            DrawRectangle(cellRect.x, cellRect.y, cellRect.width, cellRect.height, Fade(DARKGRAY, 0.1f));
            DrawText(TextFormat("Player %d: %s", users[i].player_id, users[i].username), cellRect.x + 4, cellRect.y + 4, 18, LIGHTGRAY);
        }
    }
}