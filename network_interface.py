import ctypes
import time

##functions for client side
def init_function_client_lib():
    library_definition = ctypes.CDLL('./client_prog.so')
    library_definition.open_socket.argtypes = [ctypes.c_char_p]
    library_definition.send_packet.argtypes = [ctypes.c_char_p, ctypes.c_uint32]
    library_definition.recv_packet.argtypes = [ctypes.c_uint32]
    library_definition.recv_packet.restype = ctypes.c_char_p
    return library_definition

#test function for testing ctypes library access
def test_function(library_definition):
    i = ctypes.c_int(5)
    return library_definition.my_function(i)

def setup_server_search(library_definition):
    library_definition.setup_server_search()
    return
def ping_servers(library_definition):
    library_definition.ping_servers()
    return
def end_server_listen(library_definition):
    library_definition.end_server_listen()
    return
def open_socket(library_definition, ip_addr):
    library_definition.open_socket(ctypes.c_char_p(ip_addr))
    return
def close_socket(library_definition):
    library_definition.close_socket()
    return
def send_packet(library_definition, packet):
    library_definition.send_packet(ctypes.c_char_p(packet), len(packet))
    return
def recv_packet(library_definition):
    max_len = 9
    return library_definition.recv_packet(ctypes.c_uint32(max_len))


##functions for serverside
def init_function_server_lib():
    library_definition = ctypes.CDLL('./server_prog.so')
    #library_definition.send_all_packet.argtypes = [ctypes.c_char_p, ctypes.c_uint32]
    library_definition.send_oponent_packet.argtypes = [ctypes.c_char_p, ctypes.c_uint32]
    library_definition.recv_oponent_packet.argtypes = [ctypes.c_uint32]
    library_definition.recv_oponent_packet.restype = ctypes.c_char_p
    return library_definition

def open_server_broadcast_handler(library_definition):
    library_definition.open_server_broadcast_handler()
    return

def close_server(library_definition):
    library_definition.close_server()
    return

def open_server(library_definition):
    library_definition.open_server()
    return

#spectate not implemented 
#def send_all_packet(library_definition):
#    
#    return

def send_oponent_packet(library_definition, packet):
    library_definition.send_all_packet(ctypes.c_char_p(packet), len(packet))
    return

def recv_oponent_packet(library_definition):
    max_len = 9
    return library_definition.recv_oponent_packet(ctypes.c_uint32(max_len))


##test program section:##
client_lib = init_function_client_lib()
server_lib = init_function_server_lib()

print(test_function(client_lib))
