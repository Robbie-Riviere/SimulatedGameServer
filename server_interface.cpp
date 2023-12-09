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
    freeaddrinfo(cp_servinfo);
    printf("listener: waiting to recvfrom...\n");

    return;
}

void* client_ping_response(void* thread_package){
    //listen for client ping:
    char ping_message[CLIENT_PING_MST_LENGTH];
    cp_addr_len = sizeof cp_their_addr;
    if ((cp_numbytes = recvfrom(cp_sock, ping_message, CLIENT_PING_MST_LENGTH , 0,
        (struct sockaddr *)&cp_their_addr, &cp_addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("listener: got packet from %s\n", inet_ntop(cp_their_addr.ss_family, get_in_addr((struct sockaddr *)&cp_their_addr), recent_client_addr, sizeof recent_client_addr));
    printf("listener: packet is %d bytes long\n", cp_numbytes);
    ping_message[cp_numbytes-1] = '\0';
    printf("listener: packet contains \"%s\"\n", ping_message);

    //respond to client ping, by opening a temporary port, pinging client and closing port
    respond_to_client_ping();
    return nullptr;
}

//helper function that opens a udp socket to most recent client and replies with server reply
void respond_to_client_ping(){
    //ssr_ is for server side response
    int ssr_sock;
    struct sockaddr_in ssr_their_addr; // connector's address information
    struct hostent *ssr_he;
    int ssr_numbytes;
    
    char* correct_client_addr = recent_client_addr;
    for (size_t i = 0; i < strnlen(recent_client_addr, 30); i++)
    {
        if (recent_client_addr[i] == ':'){
            correct_client_addr = (char*)(recent_client_addr + i + 1);
        }
    }
    printf("%s", correct_client_addr);
    
    if ((ssr_he=gethostbyname(correct_client_addr)) == NULL) {
        perror("client_interface:gethostbyname");
        exit(1);
    }
    //todo see if AF_INET can be unspec
    if ((ssr_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("client_interface:socket");
        exit(1);
    }

    int reuse_port = 1;
    if (setsockopt(ssr_sock, SOL_SOCKET, SO_REUSEPORT, &reuse_port,
        sizeof reuse_port) < 0) {
        perror("client_interface:setsockopt()");
        exit(1);
    }
    ssr_their_addr.sin_family = AF_UNSPEC;
    ssr_their_addr.sin_port = htons(atoi(SERVER_RESPONSE_PORT));//set target port for server response
    ssr_their_addr.sin_addr = *((struct in_addr *)ssr_he->h_addr);
    memset(ssr_their_addr.sin_zero, '\0', sizeof ssr_their_addr.sin_zero);

    //send response I am server
    char* send_str = (char*)"am server";
    if ((ssr_numbytes=sendto(ssr_sock, send_str, strlen(send_str), 0,
             (struct sockaddr *)&ssr_their_addr, sizeof ssr_their_addr)) == -1) {
        exit(1);
    }
    printf("sent %d bytes to %s\n", ssr_numbytes,
        inet_ntoa(ssr_their_addr.sin_addr));
    close(ssr_sock);
}

//close broadcast message port
//close listener thread
void close_server_broadcast_handler(){
    //join client ping response thread
    close(cp_sock);
}

//open standard tcp server to listen for players to join this specific server
void open_server(){


}

//send packet to all players on server (usually gamestate packet)
void send_all_packet(){

}

//send stringified packet to oponent player
void send_oponent_packet(){

}

//update the spectator and oponent structures to contian a new player as oponent
void set_oponent(char* oponent_ip_addr){

}

//receive message from oponent player (blocking with timeout)
void recv_oponent_packet(uint8_t* buffer, uint32_t buffer_size){

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
    //open the broadcast handler to allow clients to ping server
    open_server_broadcast_handler();
    //todo delete this since it will be handled by a thread
    //wait for a ping to be heard and reply with own IP address
    client_ping_response(nullptr);
        //internally cloases the response port

    //... do game stuff

    //close the soceket listening for pings
    close_server_broadcast_handler();

}
