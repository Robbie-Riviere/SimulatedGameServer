import socket
import pickle

# import the game
from tic_tac_toe import TicTacToe

HOST = '129.21.122.47'
PORT = 8080

# connect to the host
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
print(f"\nConnected to {HOST, PORT}!")

# set up the game
player_o = TicTacToe("O")
player_coord = " "

# allow the player to suggest playing again
rematch = True


def ask_for_input():
    global player_o, player_coord

    coord = input(f"Enter coordinate: ")
    valid = player_o.check_valid(coord)

    if valid:
        player_coord = coord
    else:
        print("Invalid coordinate.")
        ask_for_input()


while rematch:
    print(f"\n\n T I C - T A C - T O E ")

    # draw the grid
    player_o.draw_grid()

    # host goes first, client receives first
    print(f"\nWaiting for other player...")
    x_symbol_list = s.recv(1024)
    x_symbol_list = pickle.loads(x_symbol_list)
    player_o.update_symbol_list(x_symbol_list)

    # the rest is in a loop; if either player has won, it exits
    while not player_o.did_win("O") and not player_o.did_win("X") and not player_o.is_draw():
        # draw grid, ask for coordinate
        print(f"\n       Your turn!")
        player_o.draw_grid()
        ask_for_input()
        player_o.edit_square(player_coord)

        # draw grid again
        player_o.draw_grid()

        # pickle the symbol list and send it
        o_symbol_list = pickle.dumps(player_o.symbol_list)
        s.send(o_symbol_list)

        # if the player won with the last move, or it's a draw, exit the loop
        if player_o.did_win("O") or player_o.is_draw():
            break

        # wait to receive the symbol list and update it
        print(f"\nWaiting for other player...")
        x_symbol_list = s.recv(1024)
        x_symbol_list = pickle.loads(x_symbol_list)
        player_o.update_symbol_list(x_symbol_list)

    # end game messages
    if player_o.did_win("O"):
        print(f"Congrats, you won!")
    elif player_o.is_draw():
        print(f"It's a draw!")
    else:
        print(f"Sorry, the host won.")

    # host is being asked for a rematch, awaiting response
    print(f"\nWaiting for host...")
    host_response = s.recv(1024)
    host_response = pickle.loads(host_response)
    client_response = "N"

    # if the host wants a rematch, then the client is asked
    if host_response == "Y":
        print(f"\nThe host would like a rematch!")
        client_response = input("Rematch? (Y/N): ")
        client_response = client_response.capitalize()
        temp_client_resp = client_response

        # let the host know what the client decided
        client_response = pickle.dumps(client_response)
        s.send(client_response)

        # if the client wants a rematch, restart the game
        if temp_client_resp == "Y":
            player_o.restart()

        # if the client said no, then no rematch
        else:
            rematch = False

    # if the host said no, then no rematch
    else:
        print(f"\nThe host does not want a rematch.")
        rematch = False

spacer = input(f"\nThank you for playing!\nPress enter to quit...\n")

s.close()
