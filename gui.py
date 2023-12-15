import tkinter as tk
import threading
import server_gui, client_gui


def server():
    global window
    window.destroy()

    server_gui.run()


def client():
    global window
    window.destroy()

    client_gui.run()


def choose_mode():
    global window
    window = tk.Tk()
    window.title("Choose a Mode!")
    label = tk.Label(window, text="\nWould you like to be a server or client?\n\n", font=('Times New Roman', 16))
    server_button = tk.Button(window, text="Server", font=('Times New Roman', 14), command=server)
    client_button = tk.Button(window, text="Client", font=('Times New Roman', 14), padx=3, command=client)
    footer = tk.Label(window, text="\n")

    label.pack()
    server_button.pack()
    client_button.pack()
    footer.pack()

    window.mainloop()


if __name__ == '__main__':
    choose_mode()
