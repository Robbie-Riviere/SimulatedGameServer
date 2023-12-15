#define _WIN32_WINNT 0x501

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define MY_SOCKET_ERROR 21
#define GETADDRINFO_ERROR 22
#define CONNECT_ERROR 24

#define IP_STRN_LEN 46
#define DEFAULT_IP "127.0.0.1"
#define IP_ADDR "192.168.1.187"
#define PORT "2468"
#define BACKLOG 10
#define STR_BUF_LEN 256

int main() {
    WSADATA wsaData;
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        return MY_SOCKET_ERROR;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(DEFAULT_IP, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        WSACleanup();
        return GETADDRINFO_ERROR;
    }

    // Loop through all the results and connect to the first one we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, (int)p->ai_addrlen) == -1) {
            closesocket(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }

    // Client is set up

    char echo[25];
    recv(sockfd, echo, 25, 0);
    printf("%s", echo);

    while (1) {
        send(sockfd, echo, 25, 0);
    }

    printf("connected\n");
    closesocket(sockfd);
    freeaddrinfo(servinfo);
    WSACleanup();
    return 0;
}
