#include <stdint.h>

//standardize a couple operations for each OS server type
//Windows OS needs to handle these 3 functions 
typedef struct addr_struct{
    uint8_t* ip_addr;
    uint16_t port_number;
}addr_struct_t;

addr_struct_t* search_for_servers();

void open_connection();

void send_buffer(uint32_t* buffer, uint32_t buffer_length);

void recv_data(uint32_t* buffer, uint32_t buffer_length);

void close_connection();