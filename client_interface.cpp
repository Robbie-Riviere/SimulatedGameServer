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


int main(int argc, char const *argv[])
{
    #ifdef UNRECOGNIZED_OS_ERROR
        return UNRECOGNIZED_OS_ERROR;
    #endif


    int sockfd;
    struct sockaddr_in addr;
    char buffer[1024];
    socklen_t addr_size;
    char* broadcast_ip = (char*)"129.21.254.255";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int enable = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(int));

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(PORT));
    addr.sin_addr.s_addr = INADDR_BROADCAST;

    int broadcast = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    
    bzero(buffer, 1024);
    strcpy(buffer, "Hello World!");
    sendto(sockfd, buffer, 1024, MSG_CONFIRM, (struct sockaddr*)&addr, sizeof(addr));
    printf("[+]Data send: %s\n", buffer);

    bzero(buffer, 1024);
    addr_size = sizeof(addr);
    recvfrom(sockfd, buffer, 1024, MSG_WAITALL, (struct sockaddr*)&addr, &addr_size);
    printf("[+]Data recv: %s\n", buffer);

    return 0;
}
