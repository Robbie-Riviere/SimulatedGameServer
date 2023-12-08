#pragma once
#include <stdint.h>

//structures to handle everyone who is part of the server
//structure for players (one needs to be active player the rest are spectators)

//setup server to listen for broadcast messages
//spin thread that listens for messages of the right type and replies with server info
void open_server_broadcast_handler();

//close broadcast message port
//close listener thread
void close_server_broadcast_handler();

//open standard tcp server to listen for players to join this specific server
void open_server();

//send packet to all players on server (usually gamestate packet)
void send_all_packet();

//send stringified packet to oponent player
void send_oponent_packet();

//update the spectator and oponent structures to contian a new player as oponent
void set_oponent(char* oponent_ip_addr);

//receive message from oponent player (blocking with timeout)
void recv_oponent_packet(uint8_t* buffer, uint32_t buffer_size);

//starts a thread that:
//needs to listen for rtt packets and reply to them
//needs to handle if a spectator leaves the connection.
void spectator_handler();