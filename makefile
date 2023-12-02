CLIENT_SOURCE = example_client_linux.cpp
SERVER_SOURCE = example_server_linux.cpp
SOURCE = $(CLIENT_SOURCE) $(SERVER_SOURCE)
SERVER_PROG = server
CLIENT_PROG = client
SERVER_OBJ = $(patsubst %.cpp, %.o, $(SERVER_SOURCE))
CLIENT_OBJ = $(patsubst %.cpp, %.o, $(CLIENT_SOURCE))
#compiler directives
CPP = g++
CPPFLAGS = -g -pedantic -lpthread

#.SILENT:
.PHONY: all
all: $(SERVER_PROG) $(CLIENT_PROG)

$(CLIENT_PROG): $(CLIENT_OBJ)
	$(CPP) $(CLIENT_OBJ) -o $(CLIENT_PROG) $(CFLAGS)

$(SERVER_PROG): $(SERVER_OBJ)
	$(CPP) $(SERVER_OBJ) -o $(SERVER_PROG) $(CFLAGS)

.c.o:
	$(CPP) $(CPPFLAGS) $<

.PHONY: clean
clean:
	-rm -f $(CLIENT_PROG)
	-rm -f $(SERVER_PROG)
	-rm -f *.o