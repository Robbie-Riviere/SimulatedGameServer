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
#include "macros.h"
#include "pingheader.hpp"
int sockfd;
struct addrinfo *res; // will point to results
struct addrinfo hints, *servinfo, *p;
struct sockaddr_in broadcast_struct;
struct sockaddr_storage their_addr;
socklen_t sin_size;
struct sigaction sa;
int yes = 1;
int rv;
char *server_ip_addr = DEFAULT_IP;
char *broadcast_server_addr = DEFAULT_IP;

addr_struct_t *search_for_servers()
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(broadcast_server_addr, "5121", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(GETADDRINFO_ERROR);
	}

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
					   sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("client: bind error");
			continue;
		}
		break;
	}
	if (p == NULL)
	{
		fprintf(stderr, "client: failed to bind on server search\n");
		exit(BIND_ERROR);
	}
	broadcast_struct.sin_family = AF_UNSPEC;
	broadcast_struct.sin_port = htons(atoi("5121"));
	broadcast_struct.sin_addr.s_addr = inet_addr(DEFAULT_IP);
	char *buffer = (char *)"hello world";
	// bzero(buffer, size);
	// strcpy(buffer, "hello world");
	sendto(sockfd, buffer, strnlen(buffer, 25), 0, (struct sockaddr *)&broadcast_struct, sizeof(broadcast_struct));
	printf("sent \n");
	return nullptr;
}

void open_connection()
{ // todo add server_ip_addr as argument
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(server_ip_addr, "5121", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(GETADDRINFO_ERROR);
	}

	// loop through all the results and connect to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
							 p->ai_protocol)) == -1)
		{
			perror("client: socket");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}
}

void send_buffer(uint32_t *buffer, uint32_t buffer_length)
{
	send(sockfd, buffer, buffer_length, 0);
}

void recv_data(uint32_t *buffer, uint32_t buffer_length)
{
	recv(sockfd, buffer, buffer_length, 0);
}

void close_connection()
{
	freeaddrinfo(servinfo);
	close(sockfd);
}
int32_t icmp_rtt(char *addr)
{
	icmplib::PingResult res = icmplib::Ping(std::string(addr));
	switch (res.response)
	{
	case icmplib::PingResponseType::Success:
		return (int32_t)res.delay;
	default:
		return -1;
	}
	return 0;
}