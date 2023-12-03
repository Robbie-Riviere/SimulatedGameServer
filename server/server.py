# Import Statements:
import socket


# Global Variables
port = 6969


# Get the IP address associated with the hostname
def get_local_ip_address():
    try:
        return socket.gethostbyname(socket.gethostname())
    except socket.error as e:
        print(f"Error: {e}")
        return 0


# Host a server on local IP address on port 6969
def host_server():
    host = get_local_ip_address()
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    server_socket.bind((host, port))

    print(f"Server broadcasting on {host}:{port}")

    while True:
        message, client_address = server_socket.recvfrom(1024)
        print(f"Received broadcast from {client_address}: {message.decode()}")

        # Send acknowledgment to the client
        ack_message = "Server Acknowledgment"
        server_socket.sendto(ack_message.encode(), client_address)