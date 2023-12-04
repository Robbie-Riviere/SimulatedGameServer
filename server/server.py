# Import Statements:
import socket
import time


# Global Variables
port = 6969


# Get the IP address associated with the hostname
def get_local_ip_address():
    try:
        return socket.gethostbyname(socket.gethostname())
    except socket.error as e:
        print(f"Error: {e}")
        return 0


def host_server():

    # Create UDP socket as local host
    host = get_local_ip_address()
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind((host, port))
    server_socket.settimeout(300)

    print("-------------- Searching for Clients --------------")
    print(f"Server Address: IP: {host} | Port: {port}")

    try:
        while True:
            data, client_address = server_socket.recvfrom(1024)
            print(f"\t> Client Found: @{client_address[0]}")

            # Record the time when acknowledgment is sent
            send_time = time.perf_counter()

            # Send acknowledgment to the client
            server_socket.sendto("ACK".encode(), client_address)

            # Receive acknowledgment from the client
            ack_received_time = time.perf_counter()

            # Calculate and display the round-trip time
            round_trip_time = (ack_received_time - send_time) * 1000
            print(f"\t\t > RTT: {round_trip_time:.2f}ms")

    except socket.timeout:
        print("---------------------------------------------------")