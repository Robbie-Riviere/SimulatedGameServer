CLIENT_SOURCE = client_interface.cpp client_interface.hpp
SERVER_SOURCE = server_interface.cpp server_interface.hpp
SOURCE = $(CLIENT_SOURCE) $(SERVER_SOURCE)
SERVER_PROG = server_prog.so
CLIENT_PROG = client_prog.so
SERVER_OBJ = $(patsubst %.cpp, %.o, $(SERVER_SOURCE))
CLIENT_OBJ = $(patsubst %.cpp, %.o, $(CLIENT_SOURCE))
#compiler directives
CPP = g++
CPPFLAGS = -g -pedantic -lpthread -fPIC


#.SILENT:
.PHONY: all
all: $(SERVER_PROG) $(CLIENT_PROG)

$(CLIENT_PROG): $(CLIENT_OBJ)
	$(CPP) $(CLIENT_OBJ) -shared -o $(CLIENT_PROG) $(CPPFLAGS)

$(SERVER_PROG): $(SERVER_OBJ)
	$(CPP) $(SERVER_OBJ) -shared -o $(SERVER_PROG) $(CPPFLAGS)

.c.o:
	$(CPP) $(CPPFLAGS) -c

.PHONY: clean
clean:
	-rm -f $(CLIENT_PROG)
	-rm -f $(SERVER_PROG)
	-rm -f *.o