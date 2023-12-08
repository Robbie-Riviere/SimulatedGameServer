#pragma once
#include <stdint.h>

//structures to hold all current servers as volatile

//structures to hold current game socket

//create broadcast socket
//create listening thread
void setup_server_search(); 

//send a new message asking broadcast if they are server
void ping_servers();

//close broadcast port and listening thread
void end_server_listen();

//open a direct socket with selected port
void open_socket(char* ip_addr);

//close currently active game socket
void close_socket();

//calculate the round trip time of the currently connected server
uint32_t test_rtt();

//send stringified packet 
void send_packet(uint8_t* packet, uint32_t packet_length);

//receive stringified packet
void recv_packet(uint8_t* packet_buffer, uint32_t buffer_len);