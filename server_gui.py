import tkinter as tk
from subprocess import call


def run():
    window = tk.Tk()
    window.title("Server")
    window.geometry("500x500")

    canvas = tk.Canvas(window)
    canvas.pack()

    canvas_text = canvas.create_text(200, 50, text='', font=("Times New Roman", 18))

    test_string = "Creating Game..."
    delta = 100
    delay = 0
    for i in range(len(test_string) + 1):
        s = test_string[:i]
        update_text = lambda string=s: canvas.itemconfigure(canvas_text, text=string)
        canvas.after(delay, update_text)
        delay += delta

    delay = 3750
    for i in range(len(test_string) + 1, -1, -1):
        s = test_string[:i]
        update_text = lambda string=s: canvas.itemconfigure(canvas_text, text=string)
        canvas.after(delay, update_text)
        delay += delta

    new_string = "Waiting for Players..."
    delay = 6000
    for i in range(len(new_string) + 1):
        s = new_string[:i]
        update_text = lambda string=s: canvas.itemconfigure(canvas_text, text=string)
        canvas.after(delay, update_text)
        delay += delta

    call(["python", "host.py"])

    window.mainloop()
