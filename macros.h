#define IP_STRN_LEN     46
#define DEFAULT_IP (char*)"127.0.0.1"
#define BROADCAST_IP "255.255.255.255"
#define BROADCAST_SEARCH_PORT (char*)"2468" //port for server to listen for client
#define GAME_PORT (char*)"2469" //port to run the game over
#define SERVER_RESPONSE_PORT (char*)"2470" //port for client to listen for servers responding
#define BACKLOG 10
#define SERVER_CHECK (char*)"u server?"
#define SERVER_CHECK_LEN strnlen(SERVER_CHECK, 11)
#define SERVER_RESPONSE (char*)"am server!"
#define SERVER_RESPONSE_LEN strnlen(SERVER_RESPONSE, 12)

#define SERVER_RESPONSE_MSG_LENGTH 30
#define CLIENT_PING_MST_LENGTH 30

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