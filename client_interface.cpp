#include "client_interface.hpp"

void* server_searching(void* thread_package);

//create broadcast socket
//create listening thread
void setup_server_search(){
    //char broadcast = '1'; // if that doesn't work, try this
    if ((broadcast_he=gethostbyname(BROADCAST_IP)) == NULL) {  //todo switch DEFAULT_IP to BROADCAST_IP
        perror("client_interface:gethostbyname");
        exit(1);
    }

    if ((broadcast_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("client_interface:socket");
        exit(1);
    }

    //set socket options to be broadcast
    if (setsockopt(broadcast_sock, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) < 0) {
        perror("client_interface:setsockopt()");
        exit(1);
    }
    broadcast_their_addr.sin_family = AF_UNSPEC;
    broadcast_their_addr.sin_port = htons(atoi(BROADCAST_SEARCH_PORT));//set target port
    broadcast_their_addr.sin_addr = *((struct in_addr *)broadcast_he->h_addr);
    memset(broadcast_their_addr.sin_zero, '\0', sizeof broadcast_their_addr.sin_zero);

    //todo spin thread that runs on the "server_searching(void*)"
}

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//pthread thread that waits for replies with timeout
void* server_searching(void* thread_package){
    //sr_ is for server response
    //open udp listener socket for all servers to reply to
    int sr_sock;
    struct addrinfo sr_hints, *sr_servinfo, *sr_p;
    int sr_rv;
    int sr_numbytes;
    struct sockaddr_storage sr_their_addr;
    socklen_t sr_addr_len;
    char server_addr[INET6_ADDRSTRLEN];

    memset(&sr_hints, 0, sizeof sr_hints);
    sr_hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
    sr_hints.ai_socktype = SOCK_DGRAM;
    sr_hints.ai_flags = AI_PASSIVE; // use my IP

    if ((sr_rv = getaddrinfo(NULL, SERVER_RESPONSE_PORT, &sr_hints, &sr_servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(sr_rv));
        return nullptr;
    }

    // loop through all the results and bind to the first we can
    for(sr_p = sr_servinfo; sr_p != NULL; sr_p = sr_p->ai_next) {
        if ((sr_sock = socket(sr_p->ai_family, sr_p->ai_socktype,
                sr_p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sr_sock, sr_p->ai_addr, sr_p->ai_addrlen) == -1) {
            close(sr_sock);
            perror("listener: bind");
            continue;
        }
        break;
    }

    if (sr_p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return nullptr;
    }
    freeaddrinfo(sr_servinfo);
    printf("listener: waiting to recvfrom...\n");
    char server_response_buffer[SERVER_RESPONSE_MSG_LENGTH];
    sr_addr_len = sizeof sr_their_addr;

    //now that udp listener port is set up, listen for server replies and print them as received
    //todo have volatile bool listener running respond to acts of the thread
    //while (listener_running){
    if ((sr_numbytes = recvfrom(sr_sock, server_response_buffer, SERVER_RESPONSE_MSG_LENGTH, 0, (struct sockaddr *)&sr_their_addr, &sr_addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
    printf("listener: got packet from %s\n", inet_ntop(sr_their_addr.ss_family, get_in_addr((struct sockaddr *)&sr_their_addr), server_addr, sizeof server_addr));
    printf("listener: packet is %d bytes long\n", sr_numbytes);
    server_response_buffer[sr_numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", server_response_buffer);
    //}
    close(sr_sock);
    return nullptr;
    //loop handling server replies
}

//send a new message asking broadcast if they are server
void ping_servers(){
    //send query "are you game server?"
    char* send_str = (char*)"u server?";//todo make this more professional
    if ((broadcast_numbytes=sendto(broadcast_sock, send_str, strlen(send_str), 0,
             (struct sockaddr *)&broadcast_their_addr, sizeof broadcast_their_addr)) == -1) {
        exit(1);
    }
    printf("sent %d bytes to %s\n", broadcast_numbytes,
        inet_ntoa(broadcast_their_addr.sin_addr));
}

//close broadcast port and listening thread
void end_server_listen(){
    //todo join spun listening thread
    close(broadcast_sock);
    return;
}

//open a direct socket with selected addr
void open_socket(server_list_t* ip_addr){

}

//close currently active game socket
void close_socket(){

}

//send stringified packet 
void send_packet(uint8_t* packet, uint32_t packet_length){
    
}

//receive stringified packet
void recv_packet(uint8_t* packet_buffer, uint32_t buffer_len){

}


int main(void)
{
    #ifdef UNRECOGNIZED_OS_ERROR
        return UNRECOGNIZED_OS_ERROR;
    #endif
    
    //openbraodcast
    setup_server_search();
    //send ping
    ping_servers();
    //listen for response
    server_searching(nullptr);
        //close
    //close broadcast
    end_server_listen();


    //open socket 

    //send packet
    //recv packet

    //close socket
    

    return 0;
}
