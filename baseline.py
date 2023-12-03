import socket
import threading
import time

def get_local_ip_address():
    try:
        # Get the hostname of the device
        hostname = socket.gethostname()

        # Get the IP address associated with the hostname
        local_ip = socket.gethostbyname(hostname)

        return local_ip
    except socket.error as e:
        print(f"Error: {e}")
        return None

def server(host, port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind((host, port))
    print(f"Server listening on {host}:{port}")

    while True:
        data, client_address = server_socket.recvfrom(1024)
        print(f"Received data from {client_address}: {data.decode()}")

        # Record the time when acknowledgment is sent
        send_time = time.perf_counter()

        # Send acknowledgment to the client
        ack_packet = "ACK"
        server_socket.sendto(ack_packet.encode(), client_address)

        # Receive acknowledgment from the client
        ack_received_time = time.perf_counter()

        # Calculate and display the round-trip time
        round_trip_time = (ack_received_time - send_time) * 1e6
        print(f"Round-trip time: {round_trip_time:.2f} microseconds")

def client(host, port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Inform the server that the client is ready
    client_socket.sendto("Hello Server".encode(), (host, port))

    # Receive acknowledgment from the server
    ack_packet, addr = client_socket.recvfrom(1024)
    print(f"Received acknowledgment from {addr}: {ack_packet.decode()}")

def main():
    port = 6969

    print("Welcome! Choose an option:")
    print("1. Be a Server")
    print("2. Be a Client")

    choice = input("Enter 1 or 2: ")

    if choice == "1":
        host = get_local_ip_address()
        server_thread = threading.Thread(target=server, args=(host, port))
        server_thread.start()
    elif choice == "2":
        host = input("Enter the server host to connect to (e.g., localhost): ")
        port = int(input("Enter the server port to connect to: "))
        client(host, port)
    else:
        print("Invalid choice. Please enter 1 or 2.")

if __name__ == "__main__":
    main()