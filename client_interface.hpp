#pragma once
#define LINUX_OS


#ifdef WINDOWS_OS
include "example_client_windows.hpp"
#endif
#ifdef LINUX_OS
//#include "example_client_linux.hpp"
#else
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
#include "macros.h"

int broadcast_sock;
struct sockaddr_in broadcast_their_addr; // connector's address information
struct hostent *broadcast_he;
int broadcast_numbytes;
int broadcast = 1;

//structures to hold all current servers as volatile
typedef struct server_list {
    char* ip_addr_server;
    server_list* next = nullptr;
} server_list_t;
volatile bool listener_running = false;

//structures to hold current game socket

//create broadcast socket
//create listening thread
void setup_server_search(); 

//send a new message asking broadcast if they are server
void ping_servers();

//close broadcast port and listening thread
void end_server_listen();

//open a direct socket with selected port
void open_socket(server_list_t* ip_addr);

//close currently active game socket
void close_socket();

//send stringified packet 
void send_packet(uint8_t* packet, uint32_t packet_length);

//receive stringified packet
void recv_packet(uint8_t* packet_buffer, uint32_t buffer_len);