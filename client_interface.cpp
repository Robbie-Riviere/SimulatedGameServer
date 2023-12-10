#include "client_interface.hpp"

void* server_searching(void* thread_package);

//create broadcast socket
//create listening thread
void setup_server_search(){
    //char broadcast = '1'; // if that doesn't work, try this
    if ((broadcast_he=gethostbyname(BROADCAST_IP)) == NULL) {
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

    //create structure to listen for servers responding
    //sr_ is for server response
    //open udp listener socket for all servers to reply to
    memset(&sr_hints, 0, sizeof sr_hints);
    sr_hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
    sr_hints.ai_socktype = SOCK_DGRAM;
    sr_hints.ai_flags = AI_PASSIVE; // use my IP
    listener_timeout.tv_sec = 1;
    listener_timeout.tv_usec = 0;

    if ((sr_rv = getaddrinfo(NULL, SERVER_RESPONSE_PORT, &sr_hints, &sr_servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(sr_rv));
        return;
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
        return;
    }
    freeaddrinfo(sr_servinfo);
    if (setsockopt(sr_sock, SOL_SOCKET, SO_RCVTIMEO, &listener_timeout, sizeof listener_timeout) < 0){
        perror("timeout set option");
    }

    //spin thread that runs on the "server_searching(void*)"
    listener_running = true;
    pthread_create(&listener_thread, NULL, &server_searching, nullptr);
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
    //now that udp listener port is set up, listen for server replies and print them as received
    printf("listener: waiting to recvfrom...\n");
    char server_response_buffer[SERVER_RESPONSE_MSG_LENGTH];
    sr_addr_len = sizeof sr_their_addr;

    while (listener_running){
        if ((sr_numbytes = recvfrom(sr_sock, server_response_buffer, SERVER_RESPONSE_MSG_LENGTH, 0, (struct sockaddr *)&sr_their_addr, &sr_addr_len)) == -1) {
            printf("server response timeout\n");
        } else {
            printf("listener: got packet from %s\n", inet_ntop(sr_their_addr.ss_family, get_in_addr((struct sockaddr *)&sr_their_addr), server_addr, sizeof server_addr));
            printf("listener: packet is %d bytes long\n", sr_numbytes);
            server_response_buffer[sr_numbytes] = '\0';
            printf("listener: packet contains \"%s\"\n", server_response_buffer);
            //todo switch this to logging servers in a structure as they come in.
        }
        sleep(0);
    }
    
    return nullptr;
}

//send a new message asking broadcast if they are server
void ping_servers(){
    //send query "are you game server?"
    char* send_str = SERVER_CHECK;
    if ((broadcast_numbytes=sendto(broadcast_sock, send_str, strlen(send_str), 0,
             (struct sockaddr *)&broadcast_their_addr, sizeof broadcast_their_addr)) == -1) {
        exit(1);
    }
    printf("sent %d bytes to %s\n", broadcast_numbytes,
        inet_ntoa(broadcast_their_addr.sin_addr));
}

//close broadcast port and listening thread
void end_server_listen(){
    //join spun listening thread
    listener_running = false;
    void* thread_return_struct;
    pthread_join(listener_thread, &thread_return_struct);
    close(sr_sock);
    //also close broadcast soceket now that pinging should be done
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
    
    sleep(2);
    ping_servers();
    sleep(8);
    //close broadcast
    end_server_listen();


    //open socket 

    //send packet
    //recv packet

    //close socket
    

    return 0;
}
