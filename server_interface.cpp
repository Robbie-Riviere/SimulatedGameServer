#include "server_interface.hpp"

void* client_ping_response(void* thread_package);
void respond_to_client_ping(); //helper function for organization

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


//setup server to listen for broadcast messages
//spin thread that listens for messages of the right type and replies with server info
void open_server_broadcast_handler(){
    //open socket to reply to client's pings
    memset(&cp_hints, 0, sizeof cp_hints);
    cp_hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
    cp_hints.ai_socktype = SOCK_DGRAM;
    cp_hints.ai_flags = AI_PASSIVE; // use my IP
    timeout_struct.tv_sec = 1;
    timeout_struct.tv_usec = 0;

    if ((cp_rv = getaddrinfo(NULL, BROADCAST_SEARCH_PORT, &cp_hints, &cp_servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(cp_rv));
        return;
    }

    // loop through all the results and bind to the first we can
    for(cp_p = cp_servinfo; cp_p != NULL; cp_p = cp_p->ai_next) {
        if ((cp_sock = socket(cp_p->ai_family, cp_p->ai_socktype,
                cp_p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(cp_sock, cp_p->ai_addr, cp_p->ai_addrlen) == -1) {
            close(cp_sock);
            perror("listener: bind");
            continue;
        }
        break;
    }

    if (cp_p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return;
    }

    if (setsockopt(cp_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout_struct, sizeof timeout_struct) < 0){
        perror("timeout set option");
    }
    freeaddrinfo(cp_servinfo);
    server_open = true;
    //start thread that runs client ping response
    if (pthread_create(&(ping_response_thread), NULL, &client_ping_response, nullptr)){
        perror("thread init");
    }
    return;
}

void* client_ping_response(void* thread_package){
    //todo needs to run on thread
    while (server_open){
        //listen for client ping:
        char ping_message[CLIENT_PING_MST_LENGTH+1];
        cp_addr_len = sizeof cp_their_addr;
        if ((cp_numbytes = recvfrom(cp_sock, ping_message, CLIENT_PING_MST_LENGTH , 0,
            (struct sockaddr *)&cp_their_addr, &cp_addr_len)) == -1) {
            printf("recvfrom_timeout\n");
        } else {
            printf("listener: got packet from %s\n", inet_ntop(cp_their_addr.ss_family, get_in_addr((struct sockaddr *)&cp_their_addr), recent_client_addr, sizeof recent_client_addr));
            printf("listener: packet is %d bytes long\n", cp_numbytes);
            ping_message[cp_numbytes] = '\0';
            printf("listener: packet contains \"%s\"\n", ping_message);
            //respond to client ping, by opening a temporary port, pinging client and closing port
            if (strncmp(SERVER_CHECK, ping_message, SERVER_CHECK_LEN) == 0){
                printf("ping recv\n");
                respond_to_client_ping();
            }
        }
        sleep(0);
    }

    return nullptr;
}

//helper function that opens a udp socket to most recent client and replies with server reply
void respond_to_client_ping(){
    //ssr_ is for server side response
    int ssr_sock;
    struct addrinfo ssr_hints, *ssr_serverinfo, *ssr_p;
    int ssr_rv;
    int ssr_numbytes;

    //filter out the uneeded data before the addr
    char* correct_client_addr = recent_client_addr;
    for (size_t i = 0; i < strnlen(recent_client_addr, 30); i++)
    {
        if (recent_client_addr[i] == ':'){
            correct_client_addr = (char*)(recent_client_addr + i + 1);
        }
    }

    memset(&ssr_hints, 0, sizeof ssr_hints);
    ssr_hints.ai_family = AF_INET; // set to AF_INET to use IPv4
    ssr_hints.ai_socktype = SOCK_DGRAM;

    if ((ssr_rv = getaddrinfo(correct_client_addr, SERVER_RESPONSE_PORT, &ssr_hints, &ssr_serverinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ssr_rv));
        return;
    }

    // loop through all the results and make a socket
    for(ssr_p = ssr_serverinfo; ssr_p != NULL; ssr_p = ssr_p->ai_next) {
        if ((ssr_sock = socket(ssr_p->ai_family, ssr_p->ai_socktype,
                ssr_p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (ssr_p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return;
    }
    char* reply_message = SERVER_RESPONSE;
    if ((ssr_numbytes = sendto(ssr_sock, reply_message, SERVER_RESPONSE_LEN, 0,
             ssr_p->ai_addr, ssr_p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(ssr_serverinfo);

    printf("talker: sent %d bytes to %s\n", ssr_numbytes, correct_client_addr);
    close(ssr_sock);

    return;
}

//close broadcast message port
//close listener thread
//close accept thread
//close accept socket
void close_server(){
    //join client ping response thread
    server_open = false;
    void* thread_retrun_strcut;
    //close out the thread
    pthread_join(ping_response_thread, &thread_retrun_strcut);
    close(cp_sock);
    //close out the accepting portion of the server
    pthread_join(server_accepting_thread, &thread_retrun_strcut);
    for (client_connection_t* i = all_clients; i != nullptr; i = i->next)
    {
        close(i->socket);
    }
    close(gs_sock);
	freeaddrinfo(gs_servinfo);
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

//open standard tcp server to listen for players to join this specific server
void open_server(){
    
    memset(&gs_hints, 0, sizeof(gs_hints));
    gs_hints.ai_family = AF_UNSPEC;
    gs_hints.ai_flags = AI_PASSIVE; //setting this flag allows for all IP addresses to connect
    gs_hints.ai_socktype = SOCK_STREAM;

    if ((gs_rv = getaddrinfo(NULL, GAME_PORT, &gs_hints, &gs_servinfo)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gs_rv));
        exit(GETADDRINFO_ERROR);
    }
    printf("server_open\n");
	for (gs_p = gs_servinfo; gs_p!= NULL; gs_p = gs_p->ai_next){
		if ((gs_sock = socket(gs_p->ai_family, gs_p->ai_socktype, gs_p->ai_protocol)) == -1){
		    perror("socket");
		    continue;
		}

		if (bind(gs_sock, gs_p->ai_addr, gs_p->ai_addrlen) == -1) {
		    close(gs_sock);
		    perror("server: bind");
		    continue;
		}

		break;
	}
	if (gs_p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(BIND_ERROR);
	}

	if (listen(gs_sock, BACKLOG) == -1) {
		perror("listen");
		exit(SOCKET_ERROR);
	}

	gs_sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&gs_sa.sa_mask);
	gs_sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &gs_sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
    gs_sin_size = sizeof(gs_their_addr);
    //spin thread to continue to accep connections
    
    if (pthread_create(&server_accepting_thread, NULL, &accept_clients_connections, nullptr)){
        perror("pthread create");
    }
}

void* accept_clients_connections(void* threa_struct){
    
    //set up linked list of all clients
    all_clients = nullptr;
    client_connection_t* leaf_client = nullptr;
    client_connection_t* current_client = (client_connection_t*)malloc(sizeof(client_connection_t));
    while(server_open){
        current_client->socket = accept(gs_sock, (struct sockaddr *)&gs_their_addr, &gs_sin_size);//todo add timeout
        current_client->next = nullptr;
        if (current_client->socket == -1) {
            perror("accept");
        } else {
            if (all_clients == nullptr){
                all_clients = current_client;
                leaf_client = all_clients;
                current_client = (client_connection_t*)malloc(sizeof(client_connection_t));
            } else {
                leaf_client->next = current_client;
                leaf_client = leaf_client->next;
                current_client = (client_connection_t*)malloc(sizeof(client_connection_t));
            }
            CHECK_MALLOC(current_client, "current client malloc error")
        }
    }
    return nullptr;
}

//send packet to all players on server (usually gamestate packet)
void send_all_packet(char* buffer, int msg_size){
    for (client_connection_t* i = all_clients; i->next != nullptr; i = i->next)
    {
        send(i->socket, buffer, 25, 0);
    }
}

//send stringified packet to oponent player
void send_oponent_packet(){

}

//update the spectator and oponent structures to contian a new player as oponent
void set_oponent(char* oponent_ip_addr){

}

//receive message from oponent player (blocking with timeout)
void recv_oponent_packet(uint8_t* buffer, uint32_t buffer_size){
    recv(gs_sock, buffer, 25, 0);
}



//starts a thread that:
//needs to listen for rtt packets and reply to them
//needs to handle if a spectator leaves the connection.
void spectator_handler(){
    
}


int main(void)
{
    #ifdef UNRECOGNIZED_OS_ERROR
        return UNRECOGNIZED_OS_ERROR;
    #endif
    //open the broadcast recv port and create thread to respond to 'valid pings'
    open_server_broadcast_handler();

    //... do game stuff
    sleep(10);
    open_server();
    //close the soceket listening for pings
    sleep(10);
    close_server();
    
}
