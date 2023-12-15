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
#include <pthread.h>

#include "macros.h"

using namespace std;

int broadcast_sock;
struct sockaddr_in broadcast_their_addr; // connector's address information
struct hostent *broadcast_he;
int broadcast_numbytes;
int broadcast = 1;

//structure to hold all current servers
list<char*> server_list;

volatile bool listener_running = false;
//strucutres to hold listening socket
int sr_sock;
struct addrinfo sr_hints, *sr_servinfo, *sr_p;
int sr_rv;
int sr_numbytes;
struct sockaddr_storage sr_their_addr;
socklen_t sr_addr_len;
char server_addr[INET6_ADDRSTRLEN];
struct timeval listener_timeout;
//structure to handle listening thread
pthread_t listener_thread;

//structures for client on game server (gc is for game client)
int gc_sock;
struct addrinfo *gc_res; //will point to results
struct addrinfo gc_hints, *gc_servinfo, *gc_p;
struct sockaddr_in gc_broadcast_struct;
struct sockaddr_storage gc_their_addr;
socklen_t gc_sin_size;
struct sigaction gc_sa;
int gc_yes = 1;
int gc_rv;

extern "C" int my_function(int i);

//create broadcast socket
//create listening thread
extern "C" void setup_server_search(); 

//send a new message asking broadcast if they are server
extern "C" void ping_servers();

//close broadcast port and listening thread
extern "C" void end_server_listen();

//open a direct socket with selected port
extern "C" void open_socket(uint32_t index);

//close currently active game socket
extern "C" void close_socket();

//send stringified packet 
extern "C" void send_packet(char* packet, uint32_t packet_length);

//receive stringified packet
extern "C" char* recv_packet(uint32_t buffer_len);

//return the number of servers found
extern "C" uint32_t get_num_servers();