import tkinter as tk
from PIL import ImageTk, Image


def run():
    window = tk.Tk()
    window.title("Client")
    window.geometry("500x500")

    available_games = [tk.Button(window, text=f"Player {i}'s Game", width=50, height=7) for i in range(1, 5)]
    pings = [tk.Label(window, text=f"Ping: {i*50}", width=125, height=115, compound='center') for i in range(1, 5)]

    green_img = Image.open(r'green.png').resize((115, 115))
    yellow_img = Image.open(r'yellow.png').resize((100, 100))
    red_img = Image.open(r'red.png').resize((100, 100))
    green = ImageTk.PhotoImage(green_img)
    yellow = ImageTk.PhotoImage(yellow_img)
    red = ImageTk.PhotoImage(red_img)

    for i in range(len(available_games)):
        available_games[i].place(x=0, y=i*120)
        if int(pings[i]['text'][6:]) <= 75:
            pings[i]['image'] = green
        elif 75 < int(pings[i]['text'][6:]) <= 125:
            pings[i]['image'] = yellow
        else:
            pings[i]['image'] = red
        pings[i].place(x=370, y=i*120)

    window.mainloop()
