#include "skyelib.h"
#include <enet/enet.h>
#include <stdio.h>

/* Server */
extern ENetHost *server;
extern ENetAddress address;
extern ENetEvent event;

/* Client */
extern ENetHost *client;
extern ENetPeer *peer;
extern const char *msg;
extern char buffer[1024];
extern size_t len;

typedef enum MESSAGE_TYPE {
    MESSAGE_TYPE_HELLO = 0,
    MESSAGE_TYPE_CONNECT,
} MESSAGE_TYPE;

#pragma pack(push, 1)

typedef struct MultiplayerUser {
    ENetPeer *peer;
    int player_id;
    char username[64];
} MultiplayerUser;

typedef struct ENetMessage {
    uint8_t message_type;
    MultiplayerUser user;
} ENetMessage;

#pragma pack(pop)

#define MAX_PLAYERS 64
extern MultiplayerUser users[MAX_PLAYERS];
extern int users_count;

/* ENet Client*/
int enetclient_init(void); // Initialize the ENet client system (call once at startup)
int enetclient_connect(const char *host, unsigned short port); // Connect to a server at host:port
int enetclient_send(const void *data, size_t len); // Send a message (data with length) to the server reliably
int enetclient_poll(void *buffer, size_t maxlen, size_t *out_len); // Poll for incoming messages, returns 1 if a message is received, 0 otherwise If received, fills buffer up to maxlen, returns actual length in out_len
void enetclient_disconnect(void); // Disconnect and cleanup

/* ENet Server*/
extern int server_online; // Flag to indicate if the server is online
int enetserver_init();
void enetserver_process_message(ENetEvent *event);
void enetserver_draw_gui();
void enetserver_update();
void enetserver_start();
void enetserver_stop();