# SimulatedGameServer
Creating a server based on a user's PC that will allow anyone in range to connect to it.

The program’s functionality can be separated into both a client side and server side. On program start, the client side begins by opening a User Datagram Protocol (UDP) socket on a communication port. The client then broadcasts packets and listens on a separate thread for servers on a dedicated server port, compiling a list of servers that have sent an acknowledgement packet back. These servers are populated to the user in the GUI, allowing a player to select one to play with and start the game. As for the server side functionality, a UDP socket is also opened on a communication port. Using this port, the server then listens for broadcasted packets from clients on a separate thread, sending back acknowledgement packets to clients. In this instance, the server acts as a host for the client and waits for a player to select the server, starting the game.

