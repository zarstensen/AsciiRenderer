There are two diffrent file types a texture can be stored in. 
An Asciir texture file (.art) or an compact Asciir texture file (.cart)

the .cart file is a more compact version of the .art file. This is the type of file the engine expects when loading a texture file.
The .art file is a more readable version of the format and will be used to define what data the .cart file will hold

# .art file

contains five diffrent sections (size, symbols, foreground color, background color, comment [optional])

#### size:
defines the size of the texture.
stored as two positive integers seperated by a space at the start of the file.
These two values define the size of the texture.

> first integer (width) second integer (height)

#### symbols:
defines the symbols stored in the texture. 
stored as a grid of characters where each row is seperated by a newline.  
should appear after the size section with one or more newlines.  
this section should have the same number of columns as the width of the image and the same number of rows as the height of the image.   
the data is stored as single byte characters (ASCII-8) for each grid element seperated by a single space if it is not a character code (optional at end of row).  
incase of a character code there should be no spaces between the elements. 
character codes can be written as a backslash and a 2 digit hex code where the hex value will be converted to its corresponding ASCII-8 character.  
a null character (\00) is considered transparent

> 3 a characters next to each other: a a a  
> 3 a characters with spaces inbetween them: a   a   a  
> 3 a characters with null codes inbetween them: a\00a\00a  

#### foreground color:
defines the color of the symbols stored in the texture.  
stored as a grid of hex values within the range [00-FF]. Each row is seperated by a newline.  
appears after the symbols section with one or more newlines.  
rows and cols are equal to the size.  
the data is stored as positive 6 or 8 digit hex values seperated by whitespace.
The alpha value is optional and will be defaulted to FF if it is not present
>red: FF0000  
>red with an alpha value of C0: FF0000C0

#### background color:
defines the background color of the symbols stored in the texture
same as background color

### comments:
comments are not required for a texture file and do not affect the final texture.
a comment is started with two forward slashes (like c++) and ends when a newline is hit.
anything inside the comment is ignored when converted to a .cart file

## example:
```
// A 3x3 texture describing a simple square

// size section
3 3

// symbols section
╔ ═ ╗
║\00║
╚ ═ ╝

// foreground color section

//edges are red, center is transparent

FF0000 FF0000   FF0000
FF0000 00000000 FF0000
FF0000 FF0000   FF0000

// background color section

// edges background color is green, center has no background color

00FF00 00FF00   00FF00
00FF00 00000000 00FF00
00FF00 00FF00   00FF00

// end of texture file
```

# .cart file
contains four sections like the .art file (size, symbols, foreground color, background color). 
each section has the same functionality as the .art file sections. 

the size section is 8 bytes long with the first four bytes storing the width and the last four bytes storing the height of the texture.

the symbols section appears after the size section and is widht * height bytes long, with each byte representing a symbol in the texture.

the foreground section appears after the symbols section and is width * height * 4 bytes long, with sections of four bytes representing an RGBA value in the texture.

the background color functions the same as the foregorund color.

> a .cart file should always have the size [width * height * 9 + 8] (in bytes)

## example:
```
      ╔═╗║ ║╚═╝�  ��  ��  ��  �    �  ��  ��  ��  � � � � � � � � �     � � � � � � � �
```
> the example before converted to a .cart file

# conversion

a .art file can be converted to a .cart file with the python script [ArtToCart.py](../../../../scripts/ArtToCart.py)
