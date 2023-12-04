# Import Statements:
import socket
import threading
import time


#Global Variables
port = 6969
broadcast_message = "Server Discovery"
valid_servers = []


def send_packets(udp_socket,event):
    # Send all packets
    ip_addresses = [f"129.{i}.{j}.{k}" for i in range(1, 256) for j in range(1, 256) for k in range(1, 256)]
    for ip in ip_addresses:
        udp_socket.sendto(broadcast_message.encode(), (ip, port))
    event.set()


def receive_packets(udp_socket, event):
    # Listen for servers
    while not event.is_set():
        try:
            data, addr = udp_socket.recvfrom(1024)
            if data:
                print(f"\t> Server Found: @{addr[0]}")
                valid_servers.append(addr)
        except OSError:
            pass


def search_for_servers():
    # Start looking for servers
    print("-------------- Searching for Servers --------------")
    start_time = time.time()

    # Create the UDP Socket for broadcasting
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    # Create an event to signal the end of sending
    done_event = threading.Event()

    # Create send and receive thread
    send_thread = threading.Thread(target=send_packets,args=(udp_socket,done_event,))
    recv_thread = threading.Thread(target=receive_packets, args=(udp_socket,done_event,))

    # Start the threads
    print("Process: Package Sending   | Status: -ONGOING-")
    print("Process: Package Listening | Status: -ONGOING-")
    send_thread.start()
    recv_thread.start()

    # Once send thread is done, force receive thread to close
    send_thread.join()
    udp_socket.close()
    recv_thread.join()
    print("Process: Package Sending   | Status: -COMPLETED-")
    print("Process: Package Listening | Status: -COMPLETED-")
    print("---------------------------------------------------")

    # Print out the Elapsed Time:
    elapsed_time = time.time() - start_time
    minutes, seconds = divmod(int(elapsed_time), 60)
    print(f"Elapsed Search Time: {minutes} minutes {seconds} seconds")
    return valid_servers