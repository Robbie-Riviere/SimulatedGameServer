import tkinter as tk


def run():
    window = tk.Tk()
    window.title("Server")
    window.geometry("500x500")

    text_box = tk.Text(window, width=62, height=30)
    users = [tk.Button(window, text=f"Player {i} Connected...\n", width=50, height=7) for i in range(1, 5)]

    for i in range(len(users)):
        text_box.insert(tk.END, users[i]['text'])

    text_box.place(x=0, y=0)
    window.mainloop()
