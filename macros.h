#define IP_STRN_LEN     46
#define DEFAULT_IP (char*)"127.0.0.1"
#define IP_ADDR "192.168.1.187"
#define PORT (char*)"2468"
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