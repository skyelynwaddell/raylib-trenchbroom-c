#include "skyelib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

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
    if (enet_initialize() != 0) {
        fprintf(stderr, "ENet initialization failed.\n");
        CloseWindow();
        return false;
    }
    atexit(enet_deinitialize);

    address.host = ENET_HOST_ANY;
    address.port = SERVER_PORT;

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (!server) {
        fprintf(stderr, "Failed to create ENet server host.\n");
        CloseWindow();
        return false;
    }

    printf("Server listening on %x:%u\n", address.host, address.port);
    return true;
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
            if (payload_len < sizeof(MultiplayerUser)) {
                printf("Received CONNECT message with invalid length: %zu\n", payload_len);
                return;
            }

            MultiplayerUser user = {0};
            memcpy(&user, payload, sizeof(MultiplayerUser));

            int player_id = users_count++; // Assign a new player ID
            user.player_id = player_id; // Set the player ID in the user struct

            // Store or process the user data as needed
            users[player_id] = user;
            printf("Player connected: ID=%d, Username=%s\n", player_id, user.username);
            break;



        // case MESSAGE_TYPE_HELLO:
        //     printf("Received HELLO message: %s\n", (char*)payload);
        //     // Your original logic starts here
        //     int received_int;
        //     memcpy(&received_int, payload, sizeof(int));
        //     const char *received_string = (const char *)(payload + sizeof(int));

        //     MultiplayerUser *sender = (MultiplayerUser *)event->peer->data;

        //     // 🔒 Safety checks before printf
        //     const char *username = sender ? sender->username : "<unknown>";
        //     size_t max_str_len = payload_len - sizeof(int);

        //     if (max_str_len <= 0 || memchr(received_string, '\0', max_str_len) == NULL) {
        //         printf("Warning: received string is not null-terminated or too short!\n");
        //         return;
        //     }

        //     printf("Received int: %d, string: %s from sender: %s \n",
        //            received_int, received_string, username);

            // ✅ Safe options for message response

            // reply to sender
            // enet_peer_send(event->peer, 0, packet);

            // send to all users including sender
            //enet_host_broadcast(server, 0, packet);

            // send to all users excluding sender
            /*
            for (size_t i = 0; i < server->peerCount; ++i)
            {
                ENetPeer *p = &server->peers[i];
                if (p->state == ENET_PEER_STATE_CONNECTED && p != event->peer)
                {
                    enet_peer_send(p, 0, packet);
                }
            }
            */

            // break;

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
                enet_packet_destroy(event.packet); // Free the packet after processing

                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("Client disconnected.\n");
                event.peer->data = NULL;
                break;

            default: break;
        }
    }
}


static int x = 10;
static int y = 10;
static int line_gap = 20;

static void newline(){ y+=line_gap; }

/*
draw
Draws the server status and messages on the screen.
*/
void enetserver_draw_gui()
{
    x = 10;
    y = 10;

    // --- Render ---
    BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawText("Skyesrc Server", x, y, 20, LIGHTGRAY); newline();
        if (GuiButton((Rectangle){ 30, 400, 200, 50 }, GuiIconText(server_online ? ICON_PLAYER_STOP : ICON_PLAYER_RECORD, server_online ? "Stop Server" : "Start Server"))) {
            
            if (server_online) {

                // stop server
                if (server != NULL) {
                    enet_host_destroy(server);
                    server = NULL;
                }
                users_count = 0;
                memset(users, 0, sizeof(users));
                server_online = false;
                printf("Server stopped.\n");

            } else {

                // start server
                if (enetserver_init()) {
                    server_online = true;
                    printf("Server started.\n");

                } else {
                    printf("Failed to start server.\n");
                }
            }
        } newline();

        if (server_online)
        {
            DrawText("Server is online", x, y, 20, GREEN); newline();
            DrawText(TextFormat("Server listening on %x:%u\n", address.host, address.port),x,y,20, LIGHTGRAY); newline();
            DrawText(TextFormat("Connected players: %d", users_count), x, y, 20, LIGHTGRAY); newline();

            // Display connected users
            for (int i = 0; i < users_count; i++) {
                DrawText(TextFormat("Player %d: %s", users[i].player_id, users[i].username), x, y, 20, LIGHTGRAY);
                newline();
            }
        }
        else
        {
            DrawText("Server is offline", x, y, 20, RED); newline();
        }

    EndDrawing();
}