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
#include "example_server_linux.hpp"
#include "macros.h"

int sockfd;
struct addrinfo *res; //will point to results

int numbytes;
struct addrinfo hints, *servinfo, *p;
struct sockaddr_storage their_addr;
socklen_t sin_size;
struct sigaction sa;
int yes = 1;
int rv;
int new_fd[1];

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void open_connection(){
	memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(DEFAULT_IP, "5121", &hints, &servinfo)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(GETADDRINFO_ERROR);
    }

	for (p = servinfo; p!= NULL; p = p->ai_next){
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
		    perror("socket");
		    continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		        sizeof(int)) == -1) {
		    perror("setsockopt");
		    exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
		    close(sockfd);
		    perror("server: bind");
		    continue;
		}

		break;
	}
	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(BIND_ERROR);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(SOCKET_ERROR);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	sin_size = sizeof(their_addr);
	new_fd[0] = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd[0] == -1) {
		perror("accept");
	}
}

void send_buffer(uint32_t* buffer, uint32_t buffer_length){
	send(new_fd[0], buffer, buffer_length, 0);
}

void recv_buffer(){
	
}

void close_connection(){
	close(sockfd);
	freeaddrinfo(servinfo);
}