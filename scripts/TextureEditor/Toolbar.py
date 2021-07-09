from tkinter import *
from tkinter import ttk
import tkinter as tk
from ColorPicker import ColorPicker, RGBA


class Toolbar(ttk.Frame):
    def __init__(self, root):
        super().__init__(root)

        self.root = root

        self.columnconfigure([1, 2, 3], weight=1)
        self.rowconfigure([1, 2, 3, 4, 5], weight=1)

        self.foreground_picker = ColorPicker(self, "color", width=80, height=80)
        self.background_picker = ColorPicker(self, "background color")

        self.foreground_picker.grid(column=1, row=5)
        self.background_picker.grid(column=2, row=5)



root = Tk()
root.minsize(300, 300)

root.columnconfigure([0, 1], weight=1)
root.rowconfigure(0, weight=1)

toolbar = Toolbar(root)
toolbar.grid(column=0, row=0, sticky='nsew')

root.mainloop()
