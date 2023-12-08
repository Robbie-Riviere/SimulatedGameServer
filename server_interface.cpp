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
#include "macros.h"


int main(int argc, char const *argv[])
{
    #ifdef UNRECOGNIZED_OS_ERROR
        return UNRECOGNIZED_OS_ERROR;
    #endif

    char* self_addr = (char*)"129.21.91.102";
    
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
    perror("[-]socket error");
    exit(1);
    }
    int broadcast = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast));

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(PORT));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0){
    perror("[-]bind error");
    exit(1);
    }

    bzero(buffer, 1024);
    addr_size = sizeof(client_addr);
    recvfrom(sockfd, buffer, 1024, MSG_WAITALL, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+]Data recv: %s\n", buffer);

    bzero(buffer, 1024);
    strcpy(buffer, "Welcome to the UDP Server.");
    sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    printf("[+]Data send: %s\n", buffer);

    return 0;
}
