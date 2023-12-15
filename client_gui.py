import tkinter as tk
from subprocess import call
import network_interface as nint
import time

client_lib = None


def call_game(index):
    global client_lib
    global window
    HOST = nint.get_server_by_index(client_lib, index)
    nint.end_server_listen(client_lib)

    window.destroy()

    call(["python3", "client.py", str(HOST)])


def run():
    global client_lib
    global window
    window = tk.Tk()
    window.title("Client")
    window.geometry("500x500")

    client_lib = nint.init_function_client_lib()
    nint.setup_server_search(client_lib)

    nint.ping_servers(client_lib)
    while nint.get_num_servers(client_lib) == 0:
        time.sleep(.1)

    games = [tk.Button(window, text=f"Player {i}'s Game", width=50, height=7, command=lambda: call_game(i))
             for i in range(nint.get_num_servers(client_lib))]

    for i in range(len(games)):
        games[i].after(1000, games[i].pack)

    window.mainloop()
