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

#define IP_STRN_LEN     46
#define DEFAULT_IP (char*)"127.0.0.1"
#define CURRENT_IP_ADDR (char*)"192.168.1.187"
#define PORT (char*)"2468"
#define BACKLOG 10
#define STR_BUF_LEN                 256
//errors :
//common errors 
#define MALLOC_ERROR                12
//filereading errors
#define FAIL_LOAD_FILE              14
#define UNRECOGNIZED_FILE_FORMAT    20
#define PACKET_SIZE	4096
//socket errors
#define SOCKET_ERROR                21
#define GETADDRINFO_ERROR           22
#define BIND_ERROR                  23
#define CONNECT_ERROR               24
#define DATA_SIZE 65536

#define CHECK_MALLOC(pointer, string) if(!pointer){printf("Malloc Error returned %d, %s", MALLOC_ERROR, string); exit(MALLOC_ERROR);}

int sockfd;
struct addrinfo *res; //will point to results

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(){
	int numbytes;  
    //char buf[MAX_PACKET_LEN];
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(DEFAULT_IP, PORT, &hints, &servinfo)) != 0){
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
	int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		perror("accept");
	}
	
	printf("connected\n");
	//serveris set up
    char* buf = (char*)"hello world\n";
	send(new_fd, buf, strnlen(buf, 25), 0);


    
	close(sockfd);
	sockfd = new_fd;
	freeaddrinfo(servinfo);
}