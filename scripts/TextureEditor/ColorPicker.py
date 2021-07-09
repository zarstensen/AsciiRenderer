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


class ColorPicker(ttk.Frame):
    """a widget for storing and letting the user modify an rgba value"""

    def __init__(self, root, name="Color", width=20, height=20, **kwargs):
        super().__init__(root, **kwargs)

        self.root = root

        self.width = width
        self.height = height

        self.color = RGBA()
        self.name = StringVar(root, name)

        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=4)

        self.color_label = ttk.Label(self, textvariable=self.name)

        self.color_label.grid(column=0, row=0, sticky="NW")

        self.color_preview = Label(self, bg="#000000", width=width, height=height)

        self.color_preview.grid(column=0, row=1, sticky="SW", ipadx=0, ipady=0)

        self.color_preview.bind('<Configure>', self.__prevConfCallback)
        self.color_preview.bind('<1>', self.chooseColor)

    def chooseColor(self, *args):
        result = askcolor(self.color.rgbaHex(), alpha=True)
        if None not in result:
            self.color = RGBA(result[0])
            # update preview color
            self.root.after_idle(self.__setColorAsBackground)

    def getColPrevSize(self):
        # return the size of the image in the frame
        return self.color_preview.winfo_width() - (int(str(self.color_preview['borderwidth'])) + int(str(self.grid_info()['ipadx']))) * 2, \
               self.color_preview.winfo_height() - (int(str(self.color_preview['borderwidth'])) + int(str(self.grid_info()['ipady']))) * 2

    def changeSize(self, width, height):
        self.width = width
        self.height = height

        self.color_preview.configure(width=self.width, height=self.height)

    def __setColorAsBackground(self):
        width, height = self.getColPrevSize()

        background = self.__generatePngBackground(10)

        # create img with rgb values from self.color to overlay onto background
        overlay = np.zeros((height, width, 3), dtype=np.uint8)
        overlay[:] = self.color.rgb()

        background = cv2.addWeighted(background, 1 - self.color.alpha / 255, overlay, self.color.alpha / 255, 0)

        # convert np array to tkinter photoimage
        img = Image.fromarray(background)
        imgTk = ImageTk.PhotoImage(image=img)

        # prevent preview image from being freed from memory
        self.b_img = imgTk

        self.color_preview.configure(image=self.b_img)

    def __generatePngBackground(self, block_size, colors=(RGBA((154, 154, 154, 255)), RGBA((100, 100, 100, 255)))):
        width, height = self.getColPrevSize()

        # generate tiles from current size
        png_back = np.zeros((height, width, 3), dtype=np.uint8)

        for x in range(ceil(width / block_size)):
            for y in range(ceil(height / block_size)):
                cv2.rectangle(png_back, (block_size * x, block_size * y),  (block_size * (x + 1), block_size * (y + 1)),
                              colors[(y+x) % 2].rgb(), -1)

        return png_back

    def __prevConfCallback(self, event):
        # only call __prevResize if size has changed
        if (event.width, event.height) != (self.color_preview.winfo_width(), self.color_preview.winfo_height):
            self.root.after_idle(lambda: self.__prevResize())

    def __prevResize(self):
        # if preview frame was resized, a new preview image must be generated
        self.__setColorAsBackground()
