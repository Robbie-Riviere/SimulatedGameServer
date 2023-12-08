#define IP_STRN_LEN     46
#define DEFAULT_IP (char*)"127.0.0.1"
#define BROADCAST_IP "255.255.255.255"
#define BROADCAST_SEARCH_PORT (char*)"2468"
#define PORT (char*)"2469"
#define BACKLOG 10
#define STR_BUF_LEN 256


//errors :
//common errors 
#define MALLOC_ERROR                12
//filereading errors
#define FAIL_LOAD_FILE              14
#define UNRECOGNIZED_FILE_FORMAT    20

//socket errors
#define SOCKET_ERROR                21
#define GETADDRINFO_ERROR           22
#define BIND_ERROR                  23
#define CONNECT_ERROR               24

#define CHECK_MALLOC(pointer, string) if(!pointer){printf("Malloc Error returned %d, %s", MALLOC_ERROR, string); exit(MALLOC_ERROR);}