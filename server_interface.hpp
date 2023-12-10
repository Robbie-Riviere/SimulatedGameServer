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

//structures to handle everyone who is part of the server
//structure for players (one needs to be active player the rest are spectators)

//setup server to listen for broadcast messages
//spin thread that listens for messages of the right type and replies with server info
void open_server_broadcast_handler();

//close broadcast message port
//close listener thread
void close_server_broadcast_handler();

//open standard tcp server to listen for players to join this specific server
void open_server();

//send packet to all players on server (usually gamestate packet)
void send_all_packet();

//send stringified packet to oponent player
void send_oponent_packet();

//update the spectator and oponent structures to contian a new player as oponent
void set_oponent(char* oponent_ip_addr);

//receive message from oponent player (blocking with timeout)
void recv_oponent_packet(uint8_t* buffer, uint32_t buffer_size);

//starts a thread that:
//needs to listen for rtt packets and reply to them
//needs to handle if a spectator leaves the connection.
void spectator_handler();