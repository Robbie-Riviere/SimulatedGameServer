#include "pingheader.hpp"
#include "socket_api.hpp"
#include "packet_definition.h"
#include "macros.h"
#include "game.hpp"
#include <tuple>
#include <initializer_list>
#include <mutex>
#include <iostream>
#ifndef _WIN32
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
std::tuple<SOCKET, struct sockaddr> wait_for_recieve(std::string port)
{
    struct addrinfo hints;
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    struct addrinfo *result;
    struct addrinfo ret;
    SOCKET sfd;
    struct sockaddr addr;
    SOCKET final;
    int s = getaddrinfo(NULL, port.c_str(), &hints, &result);
    for (auto rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == INVALID_SOCKET)
            continue;
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen))
        {
            ret = *rp;
            int len = rp->ai_addrlen;
            final = accept(sfd, &addr, &len);
            int socklen = sizeof(addr);
            getpeername(final, &addr, &socklen);
            std::cout << "Got a response from someone!" << std::endl;
            sendto(final, SERVER_RESPONSE, SERVER_RESPONSE_LEN, 0, rp->ai_addr, rp->ai_addrlen);
            break;
        }
        sockClose(sfd);
    }
    freeaddrinfo(result);
    return {final, addr};
}
std::atomic_int num_ended = 0;
std::atomic_int first_accepted = 0;
std::atomic_int turn = 0;
std::mutex my_stopper;
std::mutex boardstate_protect;
BoardState state;
using namespace std::chrono_literals;
void client_1()
{
    int my_id = 1;
    bool accepted = false;
    {
        std::lock_guard<decltype(my_stopper)> stopper(my_stopper);
        auto [client_sock, info] = wait_for_recieve(BROADCAST_SEARCH_PORT);
        auto addr = info;
        SOCKET sock = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
        int bound;
        do
        {
            int addr_len = sizeof(addr);
            bound = bind(sock, &addr, addr_len);
        } while (bound == SOCKET_ERROR);
        listen(sock, 2);
        struct addrinfo their_info;
        SOCKET truesock = accept(sock, NULL, NULL);
        sockClose(sock);
        if (first_accepted == 0)
        {
            first_accepted = my_id;
            turn = my_id;
        }
        while (num_ended != 0)
        {

            if (turn == my_id)
            {
                std::lock_guard<decltype(boardstate_protect)> protect(boardstate_protect);
                int recvd = recv(truesock, state.state, sizeof(state.state), 0);
                if (recvd == SOCKET_ERROR)
                {
#ifdef _WIN32
                    int last_err = WSAGetLastError();
                    switch (last_err)
                    {
                    default:
                        sockClose(truesock);
                        num_ended++;
                        return;
                    }
#endif
                }
            }
            {
                std::lock_guard<decltype(boardstate_protect)> protect(boardstate_protect);
                int s = send(truesock, state.state, sizeof(state.state), 0);
#ifdef _WIN32
                int last_err = WSAGetLastError();
                switch (last_err)
                {
                default:
                    sockClose(truesock);
                    num_ended++;
                    return;
                }
#endif
            }
            std::this_thread::sleep_for(10ms);
        }
    }
    num_ended++;
}
void client_2()
{
    int my_id = 2;
    bool accepted = false;
    {
        std::lock_guard<decltype(my_stopper)> stopper(my_stopper);
        auto [client_sock, info] = wait_for_recieve(BROADCAST_SEARCH_PORT);
        auto addr = info;
        SOCKET sock = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
        int bound;
        do
        {
            int addr_len = sizeof(addr);
            bound = bind(sock, &addr, addr_len);
        } while (bound == SOCKET_ERROR);
        listen(sock, 2);
        struct addrinfo their_info;
        SOCKET truesock = accept(sock, NULL, NULL);
        sockClose(sock);
        if (first_accepted == 0)
        {
            first_accepted = my_id;
            turn = my_id;
        }
        while (num_ended != 0)
        {

            if (turn == my_id)
            {
                std::lock_guard<decltype(boardstate_protect)> protect(boardstate_protect);
                int recvd = recv(truesock, state.state, sizeof(state.state), 0);
                if (recvd == SOCKET_ERROR)
                {
#ifdef _WIN32
                    int last_err = WSAGetLastError();
                    switch (last_err)
                    {
                    default:
                        sockClose(truesock);
                        num_ended++;
                        return;
                    }
#endif
                }
            }
            {
                std::lock_guard<decltype(boardstate_protect)> protect(boardstate_protect);
                int s = send(truesock, state.state, sizeof(state.state), 0);
#ifdef _WIN32
                int last_err = WSAGetLastError();
                switch (last_err)
                {
                default:
                    sockClose(truesock);
                    num_ended++;
                    return;
                }
#endif
            }
            std::this_thread::sleep_for(10ms);
        }
    }
    num_ended++;
}
int main()
{
    sockInit();
    std::thread client1thread(client_1);
    std::thread client2thread(client_2);
    while (num_ended != 2)
    {
        // do nothing
    }
    client1thread.join();
    client2thread.join();
    sockQuit();
}