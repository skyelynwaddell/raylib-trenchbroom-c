#include "skyelib.h"

ENetHost *client = NULL;
ENetPeer *peer = NULL;

const char *msg = "Hello server!";
char buffer[1024];
size_t len;


/*
enetclient_user_login
Sends a login message to the server with the user's information.
*/
void enetclient_user_login() 
{
    // Example user login logic
    MultiplayerUser user = {0};
    user.player_id = -1; // server decides
    strncpy(user.username, "Lisa Torvak", sizeof(user.username) - 1);
    user.username[sizeof(user.username) - 1] = '\0';  // ensure null-termination

    ENetMessage message = {0};
    message.message_type = MESSAGE_TYPE_CONNECT; // Set message type
    message.user = user;

    // Send login message to server
    enetclient_send(&message, sizeof(message));
}


/*
enetclient_init
Initializes the ENet client and connects to the server.
Returns true (1) on success, false (0) on failure.
*/
int enetclient_init(void) 
{
    if (server_online == false) 
    {
        fprintf(stderr, "ENet Failed, Multiplayer mode not enabled.\n");
        return false;
    }

    if (enet_initialize() != 0) 
    {
        fprintf(stderr, "ENet initialization failed.\n");
        return false;
    }
    atexit(enet_deinitialize);

    client = enet_host_create(NULL, 1, 2, 0, 0);
    if (!client) 
    {
        fprintf(stderr, "Failed to create ENet client host.\n");
        return false;
    }

    if (!enetclient_connect(SERVER_IP, SERVER_PORT)) 
    {
        fprintf(stderr, "Failed to connect to server.\n");
        return false;
    }

    printf("ENet client initialized successfully.\n");

    enetclient_user_login();
    return true;
} 


/*
enetclient_connect
Connects to the specified server using the provided host and port.
*/
int enetclient_connect(const char *host, unsigned short port) 
{
    if (!client) return false;

    ENetAddress address;
    if (enet_address_set_host(&address, host) != 0) 
    {
        fprintf(stderr, "Invalid host address.\n");
        return false;
    }
    address.port = port;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) 
    {
        fprintf(stderr, "No available peers for connection.\n");
        return false;
    }

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) 
    {
        printf("Connected to server %s:%u\n", host, port);
        return true;
    }
    printf("Connection to server timed out or failed.\n");
    return false;
}


/*
enetclient_send
Sends data to the connected server.
*/
int enetclient_send(const void *data, size_t len) 
{
    if (!peer) return false;

    ENetPacket *packet = enet_packet_create(data, len, ENET_PACKET_FLAG_RELIABLE);
    if (!packet) return false;

    if (enet_peer_send(peer, 0, packet) != 0) 
    {
        enet_packet_destroy(packet);
        return false;
    }
    enet_host_flush(client);
    return true;
}


/*
enetclient_poll
Polls for incoming messages from the server.
*/
int enetclient_poll(void *buffer, size_t maxlen, size_t *out_len) 
{
    if (!client) return false;

    ENetEvent event;
    int ret = enet_host_service(client, &event, 0);
    if (ret > 0 && event.type == ENET_EVENT_TYPE_RECEIVE) 
    {
        size_t packet_len = event.packet->dataLength;
        if (packet_len > maxlen) packet_len = maxlen;

        memcpy(buffer, event.packet->data, packet_len);
        *out_len = packet_len;
        enet_packet_destroy(event.packet);
        return true;  // received a message
    }
    return false;  // no message
}


/*
enetclient_disconnect
Disconnects from the server and cleans up resources.
*/
void enetclient_disconnect(void) 
{
    if (peer) {
        enet_peer_disconnect(peer, 0);

        ENetEvent event;
        while (enet_host_service(client, &event, 3000) > 0) 
        {
            if (event.type == ENET_EVENT_TYPE_DISCONNECT)
            {
                printf("Disconnected from server.\n");
                break;
            }
        }
        peer = NULL;
    }

    if (client) 
    {
        enet_host_destroy(client);
        client = NULL;
    }
}


/*
send_example
An example function to send a message to the server.
Placeholder with different types of data you can send.
*/
static int send_example()
{
    uint8_t message_type = MESSAGE_TYPE_HELLO; // Example message type

    // example data to send
    int some_int = 42;
    const char *some_string = "Hello, ENet!";

    // calculate the total packet size in bytes
    size_t str_len = strlen(some_string) + 1; // +1 for null terminator
    size_t packet_size = 1 + sizeof(some_int) + str_len;;

    // store the data in a buffer
    unsigned char buffer[256];
    buffer[0] = message_type; // First byte is the message type
    memcpy(buffer + 1, &some_int, sizeof(some_int));
    memcpy(buffer + sizeof(some_int) + 1, some_string, str_len);

    // put the buffer into an ENet packet
    ENetPacket *packet = enet_packet_create(buffer, packet_size, ENET_PACKET_FLAG_RELIABLE);
    if (!packet) return false;
    
    // send the packet to the server
    enet_peer_send(peer, 0, packet);
    enet_host_flush(peer->host);
    return true;
}


/*
enetclient_update
Sends a message to the server and processes incoming messages.
This function should be called in the main game loop to keep the client updated.
*/
void enetclient_update()
{

    if (server_online == false) return;

    // Poll ENet network events non-blocking
    while (enetclient_poll(buffer, sizeof(buffer), &len)) 
    {
        // Process the received message here
        printf("Got from server: %.*s\n", (int)len, buffer);
        // You can parse messages, update game state, etc.
    }
}
