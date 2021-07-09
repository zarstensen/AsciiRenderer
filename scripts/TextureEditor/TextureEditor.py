from tkinter import *
from tkinter import ttk


def convert(*args):
    try:
        feet = float(feet_input.get())
        meters_output.set(int(0.3048 * feet * 10000.0 + 0.5)/10000.0)
    except ValueError as e:
        pass
    root.after(1, convert)


root = Tk()
root.title("feet to meters")

frame = ttk.Frame(root, padding="3 3 12 12")
frame.grid(column=0, row=0, sticky=(N, W, E, S))

root.columnconfigure(0, weight=1)
root.rowconfigure(0, weight=1)

frame.columnconfigure(0, weight=1)
frame.rowconfigure(0, weight=1)

feet_input = StringVar()
feet_entry = Entry(frame, width=7, textvariable=feet_input, font=('TkFixedFont', 2), bg="#FF0000", fg="#00FF00")
feet_entry.grid(column=2, row=1, sticky=(W, E))

meters_output = StringVar()
ttk.Label(frame, textvariable=meters_output).grid(column=2, row=2, sticky=(W, E))

ttk.Button(frame, text="Convert", command=convert).grid(column=3, row=3, sticky=W)

ttk.Label(frame, text="feet").grid(column=3, row=1, sticky=W)
#ttk.Label(frame, text="converts to").grid(column=1, row=2, sticky=E)
ttk.Label(frame, text="meters").grid(column=3, row=2, sticky=W)

for child in frame.winfo_children():
    child.grid_configure(padx=5, pady=5)

feet_entry.focus()
root.bind("<Return>", convert)
root.bind("")

root.after(1, convert)
root.mainloop()
