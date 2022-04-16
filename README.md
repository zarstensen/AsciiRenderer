(This readme is massively outdated, will be updated soon...)
# AsciiRender (New Windows input system)

This branch should fix any previous input problems from devices, when specific key combinations are pushed (eg. shift + ANY_KEY).

Previously Asciir used the [ReadConsoleInput()](https://docs.microsoft.com/en-us/windows/console/readconsoleinput) function to read device inputs. However, this function has problems with detecting key up signals when shift (or other spcific keys) are pressed whilst releasing any other key. As there is no solution to this, the [RawInput model](https://docs.microsoft.com/en-us/windows/win32/inputdev/raw-input) will instead be used for receiving device input data.

This will also allow to take input from other devices like a controller. However, it will require the console application / game to run a hidden windows application in order to use this API.

