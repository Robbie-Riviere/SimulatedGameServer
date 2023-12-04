#define _WIN32_WINNT 0x501
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32")

#define DEFAULT_IP "127.0.0.1"
#define PORT "2468"
#define BACKLOG 10
#define STR_BUF_LEN 256
#define MY_SOCKET_ERROR 21
#define GETADDRINFO_ERROR 22
#define BIND_ERROR 23
#define ACCEPT_ERROR 24
#define DATA_SIZE 65536

void sigchld_handler(int s)
{
    // No equivalent for Windows
}

int main()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return SOCKET_ERROR;
    }

    SOCKET sockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(DEFAULT_IP, PORT, &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        WSACleanup();
        return GETADDRINFO_ERROR;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET)
        {
            perror("socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&yes),
                       sizeof(int)) == SOCKET_ERROR)
        {
            perror("setsockopt");
            closesocket(sockfd);
            WSACleanup();
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, static_cast<int>(p->ai_addrlen)) == SOCKET_ERROR)
        {
            closesocket(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }
    if (p == NULL)
    {
        std::cerr << "server: failed to bind" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return BIND_ERROR;
    }

    if (listen(sockfd, BACKLOG) == SOCKET_ERROR)
    {
        perror("listen");
        closesocket(sockfd);
        WSACleanup();
        return SOCKET_ERROR;
    }

    sin_size = sizeof(their_addr);
    SOCKET new_fd = accept(sockfd, reinterpret_cast<struct sockaddr *>(&their_addr), &sin_size);
    if (new_fd == INVALID_SOCKET)
    {
        perror("accept");
        closesocket(sockfd);
        WSACleanup();
        return ACCEPT_ERROR;
    }

    printf("connected\n");

    // Server is set up
    const char *buf = "hello world\n";
    send(new_fd, buf, static_cast<int>(strlen(buf)), 0);

    closesocket(sockfd);
    closesocket(new_fd);
    freeaddrinfo(servinfo);
    WSACleanup();

    return 0;
}
