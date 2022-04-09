There are two diffrent file types a texture can be stored in. 
An Asciir texture file (.art) or an compact Asciir texture file (.cart).

The .cart file is a more compact version of the .art file. This is the type of file the engine expects when loading a texture file.
The .art file is a more readable version of the format and will be used to define what data the .cart file will hold.

# .art file

Contains five diffrent sections (size, symbols, foreground color, background color, comments [optional])

#### size:
Defines the size of the texture.
Stored as two positive integers seperated by a space at the start of the file.
These two values define the size of the texture.

> first integer (width) second integer (height)

#### symbols:
Defines the symbols stored in the texture. 
Stored as a grid of characters where each row is seperated by a newline.  
Should appear after the size section with one or more newlines.  
This section should have the same number of columns as the width of the image and The same number of rows as the height of the image.   
The data is stored as multi byte characters (UTF-8) for each grid element Seperated by a single space if it is not a character code (optional at end of row).  
Incase of a character code there should be no spaces between the elements. 
Character codes should be written as a 2 digit hex code where the hex value will be converted to its corresponding ASCII-8 character.
these should mainly be used to define transparent characters.
A null character (00) is considered transparent.

> 3 'a' characters next to each other: a a a  
> 3 'a' characters with spaces inbetween them: a   a   a  
> 3 'a' characters with null codes inbetween them: a00a00a  

#### foreground color:
Defines the color of the symbols stored in the texture.  
Stored as a grid of hex values within the range [00-FF]. Each row is seperated by a newline.  
Appears after the symbols section with one or more newlines.  
Rows and cols are equal to the size.  
The data is stored as positive 6 or 8 digit hex values seperated by whitespace.
The alpha value is optional and will be defaulted to FF if it is not present.
>red: FF0000  
>red with an alpha value of C0: FF0000C0

#### background color:
Defines the background color of the symbols stored in the texture.  
Same as background color

### comments:
Comments are not required for a texture file and do not affect the final texture.  
A comment is started with two forward slashes (like c++) and ends when a newline is hit.  
Anything inside the comment is ignored when converted to a .cart file.

## example:
```
// A 3x3 texture describing a simple square

// size section
3 3

// symbols section
╔ ═ ╗
║00 ║
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
Contains four sections like the .art file (size, symbols, foreground color, background color) and an additional header section. 
Each section has the same functionality as the .art file sections. 

each .cart file starts with a 4 byte long header containing the characters 'C A R T'. This exists in order to detect a .cart file without depending on the file extension.

A .cart file is not compressed, the data is just stored as more compact and efficient to read by the program.

The size section is 16 bytes long with the first 8 bytes storing the width and the last 8 bytes storing the height of the texture.

The symbols, foreground color and background color are merged into each other where each element has a symbol, a foreground color and a background color.
Each element is 9-12 bytes long.
> 1-4 byte: symbol  
> 4 bytes: foreground  
> 4 bytes: background  

### IMPORTANT: 
in contrast to the .art format, a .cart file stores the symbols section as row major instead of column major.

## overview:

```
HEADER (CART): 4 bytes
SIZE (WIDTH, HEIGHT): 8 * 2 bytes
...
SYMBOL: 1-4 bytes
FOREGROUND: 4 bytes
BACKGROUND: 4 bytes
...

```

## example:
```
CART              ╔ÿÿ═ÿÿ═ÿÿ╦ÿÿ═ÿÿ═ÿÿ╗ÿÿ║ÿÿ                  ║ÿÿ                  ║ÿÿ╠ÿÿ═ÿÿ═ÿÿ╬ÿÿ═ÿÿ═ÿÿ╣ÿÿ║ÿÿ                  ║ÿÿ                  ║ÿÿ╚ÿÿ═ÿÿ═ÿÿ╩ÿÿ═ÿÿ═ÿÿ╝ÿÿ
```
> the example .art file, converted to a .cart file

# conversion

A .art file can be converted to a .cart file with the python script [ArtToCart.py](../../../../scripts/ArtToCart.py)
