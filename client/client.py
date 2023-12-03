# Import Statements:
import socket
import threading

# Global Variables
port = 6969
valid_servers = []

def search_ip_in_range(start_ind, end_ind):

    # Set socket timeout & test pkt:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(0.00005)
    broadcast_message = "Server Discovery"

    # Look for an IP in a specific range:
    for i in range(start_ind, end_ind):
        for j in range(1, 255):
            for k in range(1, 255):

                server_address = f"129.{i}.{j}.{k}"
                server_location = (server_address, port)

                try:
                    # Send a discovery message to the potential server address
                    client_socket.sendto(broadcast_message.encode(), server_location)

                    # Receive acknowledgment from servers
                    data, addr = client_socket.recvfrom(1024)
                    print(f"Received acknowledgment from server {addr}: {data.decode()}")

                    # Safely add server to valid servers using a lock
                    valid_servers.append(server_address)
                    print(f"Server found at {server_address}, {port}")

                except socket.timeout:
                    pass

# Search for server
def search_for_server(num_segments = 255):

    # Create a list of threads given num_segments:
    threads = []
    for i in range(1, num_segments + 1):

        # Create the parameters for search_ip_in_range:
        start_ind = int(((i - 1) * (256 / num_segments)) + 1)
        if i == 1: start_ind = 1
        end_ind = int(i * (256 / num_segments))
        if i == num_segments: end_ind = 255

        # print(f"T{i:03d} Coverage: 129.{start_ind:03d}.001.001 to 129.{end_ind:03d}.255.255")

        thread_name = 't{}'.format(i)

        if num_segments == 255:
            new_thread = threading.Thread(target=search_ip_in_range, name=thread_name, args=(start_ind, start_ind+1))
        else:
            new_thread = threading.Thread(target=search_ip_in_range, name=thread_name, args=(start_ind,end_ind))
        threads.append(new_thread)

    # Start all threads
    for thread in threads:
        thread.start()

    # Wait for all threads to finish
    for thread in threads:
        thread.join()

    print("Search complete. Valid servers:", valid_servers)