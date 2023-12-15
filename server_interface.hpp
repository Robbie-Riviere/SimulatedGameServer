#define LINUX_OS


#ifdef WINDOWS_OS
#define RECONGNIZED_OS
include "example_server_windows.hpp"
#endif
#ifdef LINUX_OS
#define RECONGNIZED_OS
//#include "example_server_linux.hpp"
#endif
#ifdef RECONGNIZED_OS
#define UNRECONGNIZED_OS_ERROR OS_ERROR_CODE
#endif

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <time.h>
#include <list>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#include "macros.h"

//structures that handle pings from clients
//cp_ is for client ping
int cp_sock;
struct addrinfo cp_hints, *cp_servinfo, *cp_p;
int cp_rv;
int cp_numbytes;
struct sockaddr_storage cp_their_addr;
socklen_t cp_addr_len;
char recent_client_addr[INET6_ADDRSTRLEN];
struct timeval timeout_struct;

//thread bool to notify the thread when to stop listening for pings and stop responding
volatile bool server_open;
//thread structure for ping response handling
pthread_t ping_response_thread;


//structure to handle open connection to allow all clients to connect (gs is game server)
int gs_sock;
struct addrinfo *gs_res; //will point to results

int gs_numbytes;
struct addrinfo gs_hints, *gs_servinfo, *gs_p;
struct sockaddr_storage gs_their_addr;
socklen_t gs_sin_size;
struct sigaction gs_sa;
int gs_yes = 1;
int gs_rv;

//structures to handle everyone who is part of the server
typedef struct client_connection {
    int socket;
    char* ip_addr;
    bool is_oponent;
    struct client_connection* next;
} client_connection_t;

client_connection_t* all_clients;
client_connection_t* current_oponent;
//function for connection listening thread 
pthread_t server_accepting_thread;

//structure for players (one needs to be active player the rest are spectators)

//setup server to listen for broadcast messages
//spin thread that listens for messages of the right type and replies with server info
extern "C" void open_server_broadcast_handler();

//close broadcast message port
//close listener thread
//close accept thread
//close accept socket
extern "C" void close_server();

//open standard tcp server to listen for players to join this specific server
extern "C" void open_server();

//send packet to all players on server (usually gamestate packet)
extern "C" void send_all_packet(char* buffer, int msg_size);

//send stringified packet to oponent player
extern "C" void send_oponent_packet(char* buffer, int msg_size);

//update the spectator and oponent structures to contian a new player as oponent
//extern "C" void set_oponent(char* oponent_ip_addr);

//receive message from oponent player (blocking with timeout)
extern "C" char* recv_oponent_packet(uint32_t buffer_size);
