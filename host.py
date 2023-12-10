import socket
import threading
import tkinter as tk
from tkinter import messagebox

from tic_tac_toe import TicTacToe

HOST = '129.21.122.47'
PORT = 8080

# set up the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(5)

# accept a connection from the client
client_socket, client_address = s.accept()
print(f"\nConnected to {HOST,PORT}!")


def stringify():
    global board
    new_board = ""

    for i in range(3):
        for j in range(3):
            if board[i][j] == "X":
                new_board = new_board + "X"
            elif board[i][j] == "O":
                new_board = new_board + "O"
            else:
                new_board = new_board + "."

    return new_board


def draw_grid(row, col):
    global board
    if game.get_player() == "X":
        if board[row][col] == ' ':
            board[row][col] = player
            buttons[row][col].config(text=player, state=tk.DISABLED)

            game.set_player("O")
            send_data = stringify()
            print(send_data)
            client_socket.send(send_data.encode())

            threading.Thread(target=listen).start()

            winner = check_winner()
            if winner:
                messagebox.showinfo("Winner", f"Player {winner} wins!")
                restart()
            elif is_board_full():
                messagebox.showinfo("Tie", "It's a tie!")
                restart()


def restart():
    global window
    window.destroy()
    client_socket.close()


def check_winner():
    global board
    for row in range(3):
        if all(board[row][col] == player for col in range(3)) or all(board[col][row] == player for col in range(3)):
            return player
        elif all(board[row][col] == "O" for col in range(3)) or all(board[col][row] == "O" for col in range(3)):
            return "O"
    if all(board[row][row] == player for row in range(3)) or all(board[row][2 - row] == player for row in range(3)):
        return player
    elif all(board[row][row] == "O" for row in range(3)) or all(board[row][2 - row] == "O" for row in range(3)):
        return "O"
    return None


def is_board_full():
    return all(board[row][col] != ' ' for row in range(3) for col in range(3))


def update_board(updated_board):
    global board
    global buttons
    for row in range(3):
        for col in range(3):
            if board[row][col] != updated_board[(row*3)+col] and updated_board[(row*3)+col] != ".":
                buttons[row][col].config(text="O", state=tk.DISABLED)
                board[row][col] = "O"


def make_buttons():
    global buttons
    global board
    global window

    for i in range(3):
        for j in range(3):
            buttons[i][j] = tk.Button(window, text=board[i][j], font=('normal', 20), width=5, height=2,
                                      command=lambda row=i, col=j: draw_grid(row, col))
            buttons[i][j].grid(row=i, column=j)


def listen():
    received_data = client_socket.recv(1024)
    update_board(received_data.decode())

    winner = check_winner()
    if winner:
        messagebox.showinfo("Winner", f"Player {winner} wins!")
        restart()
    elif is_board_full():
        messagebox.showinfo("Tie", "It's a tie!")
        restart()

    game.set_player("X")


# set up the game
window = tk.Tk()
player = "X"
game = TicTacToe()
window.title("Tic-Tac-Toe (Host)")
board = [[' ' for _ in range(3)] for _ in range(3)]
buttons = [[None for _ in range(3)] for _ in range(3)]

make_buttons()
window.mainloop()
