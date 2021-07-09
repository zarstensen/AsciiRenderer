from tkinter import *
from tkinter import ttk
from tkcolorpicker import askcolor
from PIL import Image, ImageTk
import cv2
import numpy as np
from math import ceil


class RGBA:
    """class for storing rgba colors and converting them to different representations"""

    def __init__(self, rgba=(0, 0, 0, 255)):
        self.red = rgba[0]
        self.green = rgba[1]
        self.blue = rgba[2]
        self.alpha = rgba[3]

    def rgba(self):
        return self.red, self.green, self.blue, self.alpha

    def rgbaHex(self):
        return f"#{''.join([format(val, 'x').zfill(2) for val in self.rgba()])}"

    def rgb(self):
        return self.red, self.green, self.blue

    def rgbHex(self):
        return f"#{''.join([format(val, 'x').zfill(2) for val in self.rgb()])}"


class SymbolPicker(ttk.Frame):
    """a widget for storing and letting the user modify a character"""

    def __init__(self, root, name="Color", width=20, height=20, **kwargs):
        super().__init__(root, **kwargs)

        self.root = root

        self.width = width
        self.height = height

        

        self.name = StringVar(root, name)

        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=4)
