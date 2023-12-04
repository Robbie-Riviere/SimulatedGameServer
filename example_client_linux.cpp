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
#include "example_client_linux.hpp"

#define IP_STRN_LEN     46
#define DEFAULT_IP (char*)"127.0.0.1"
#define IP_ADDR "192.168.1.187"
#define PORT (char*)"2468"
#define BACKLOG 10
#define STR_BUF_LEN                 256

//errors :
//common errors 
#define MALLOC_ERROR                12
//filereading errors
#define FAIL_LOAD_FILE              14
#define UNRECOGNIZED_FILE_FORMAT    20

//socket errors
#define SOCKET_ERROR                21
#define GETADDRINFO_ERROR           22
#define BIND_ERROR                  23
#define CONNECT_ERROR               24

#define CHECK_MALLOC(pointer, string) if(!pointer){printf("Malloc Error returned %d, %s", MALLOC_ERROR, string); exit(MALLOC_ERROR);}

int sockfd;
struct addrinfo *res; //will point to results
struct addrinfo hints, *servinfo, *p;
struct sockaddr_storage their_addr;
socklen_t sin_size;
struct sigaction sa;
int yes = 1;
int rv;


addr_struct_t* search_for_servers(){
	
}

void open_connection(){
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(DEFAULT_IP, PORT, &hints, &servinfo)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(GETADDRINFO_ERROR);
    }

    // loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				 p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}
}

void send_buffer(uint32_t* buffer, uint32_t buffer_length){
	send(sockfd, buffer, buffer_length, 0);
}

void recv_data(uint32_t* buffer, uint32_t buffer_length){
	recv(sockfd, buffer, buffer_length, 0);
}

void close_connection(){
    freeaddrinfo(servinfo);
    close(sockfd);
}