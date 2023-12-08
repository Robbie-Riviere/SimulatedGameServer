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
#include "client_interface.hpp"

int main(void)
{
    #ifdef UNRECOGNIZED_OS_ERROR
        return UNRECOGNIZED_OS_ERROR;
    #endif
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int broadcast = 1;
    //char broadcast = '1'; // if that doesn't work, try this

    if ((he=gethostbyname(DEFAULT_IP)) == NULL) {  //todo switch DEFAULT_IP to BROADCAST_IP
        perror("client_interface:gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("client_interface:socket");
        exit(1);
    }

    //set socket options to be broadcast
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) < 0) {
        perror("client_interface:setsockopt()");
        exit(1);
    }
    their_addr.sin_family = AF_UNSPEC;
    their_addr.sin_port = htons(atoi(BROADCAST_SEARCH_PORT));//set target port
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);


    //send query "are you game server?"
    //spin thread that waits for replies

    char* send_str = (char*)"hello world";
    if ((numbytes=sendto(sockfd, send_str, strlen(send_str), 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        exit(1);
    }

    printf("sent %d bytes to %s\n", numbytes,
        inet_ntoa(their_addr.sin_addr));
    close(sockfd);

    return 0;
}
