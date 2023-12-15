#include <stdint.h>

//standardize a couple operations for each OS server type
//Windows OS needs to handle these 5 functions
void open_connection();

void send_buffer(uint32_t* buffer, uint32_t buffer_length);

void recv_data(uint32_t* buffer, uint32_t buffer_length);

void close_connection();