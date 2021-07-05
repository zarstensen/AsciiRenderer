import sys
import os
from pathlib import Path


def parseSizeSection(line_iterator):
    while True:
        line = next(line_iterator).strip()

        # skip if line is empty or a comment
        if line.startswith("//") or line.strip() == '':
            continue

        # remove white space between data entries
        size_data = [data_entry for data_entry in line.split(" ") if data_entry != '']

        if len(size_data) != 2:
            if len(size_data) > 2:
                print("Found more than 2 data entries in the size section, skipping file")
            elif len(size_data) < 2:
                print("Found less than 2 data entries in the size section, skipping file")
            return None

        # try to convert to integers
        try:
            size_data = [int(data_entry) for data_entry in size_data]
        except ValueError:
            print("Size value must be an integer, skipping file")
            return None

        # check if all data entries are positive
        for data_entry in size_data:
            if data_entry < 0:
                print("Size value must be a positive integer, skipping file")
                return None

        return size_data


def parseSymbolsSection(line_iterator, size):
    symbols = []

    while True:
        # remove newline character from the line
        line = next(line_iterator).replace("\n", "").replace("\r", "")

        # skip if line is empty or a comment
        if line.startswith("//") or line.strip() == '':
            continue

        symbols.append([])

        row_iterator = iter(line)

        # parse and append characters to the symbols table
        for i, character in enumerate(row_iterator):
            if i % 2:
                if character == ' ':
                    continue
                elif character == '\\':
                    # use next two characters as a single hex value and convert an int
                    try:
                        symbols[-1].append(int(next(row_iterator) + next(row_iterator), 16))
                    except ValueError:
                        print("key code did not evaluate to a valid hex value, skipping this file")
                        return None
                else:
                    print("missing space between symbols or character code was not formatted corretly, skipping this file")
                    return None
            else:
                symbols[-1].append(character)

        # check if the length of the row matches the size of the texture
        if len(symbols[-1]) != size[1]:
            if len(symbols[-1]) < size[1]:
                print("found too few elements in the symbols section, skipping this file")
            elif len(symbols[-1]) > size[1]:
                print("found too many elements in the symbols section, skipping this file")
            return None

        # stop when the correct size is reached
        if len(symbols) == size[0]:
            break

    return symbols


def parseColorSection(line_iterator, size, section_name):
    color_data = []

    while True:
        # remove newline character from the line
        line = next(line_iterator).strip()

        # skip if line is empty or a comment
        if line.startswith("//") or line == '':
            continue

        # remove whitespace between elements
        line = [elem for elem in line.split(' ') if elem != '']

        # make sure the row matches the size of the texture
        if len(line) != size[1]:
            if len(line) < size[1]:
                print(f"found too few elements in the {section_name} section, skipping this file")
            elif len(line) > size[1]:
                print(f"found too many elements in the {section_name} section, skipping this file")
            return None

        color_data.append([])

        # parse values in the row
        for elem in line:
            color_values = [elem[index:index+2] for index in range(0, len(elem), 2)]

            if len(color_values) != 3 and len(color_values) != 4:
                if len(color_values) < 3:
                    print(f"found too few values in an element in the {section_name} section, skipping this file")
                if len(color_values) > 4:
                    print(f"found too many values in an element in the {section_name} section, skipping this file")
                return None

            # convert hex values into integers
            for val in color_values:
                color_data[-1].append(int(val, 16))

            # add missing alpha channel
            if len(color_values) == 3:
                color_data[-1].append(0xFF)
        # stop loop when size is met
        if len(color_data) == size[0]:
            break

    return color_data


target_files = sys.argv[1:]

print("converting files ", target_files, " to .cart files")

for path in [Path(file_path) for file_path in target_files]:

    if not path.exists():
        print("WARNING: the file ", path, " does not exist, skipping this file")
        continue

    if not path.suffix == ".art":
        print("WARNING: the file ", path, " is not an Ascii texture file, conversion may fail")

    size_data = None
    symbols_data = None
    foreground_data = None
    background_data = None

    with open(path, 'r', encoding='utf-8') as art_in:

        # size section data

        print("converting ", path, " to a .cart file")

        failed_parse = False

        lines = iter(art_in.readlines())

        try:
            size_data = parseSizeSection(lines)
            if size_data is None:
                continue

            print("parsed size section")

            symbols_data = parseSymbolsSection(lines, size_data)
            if symbols_data is None:
                continue

            print("parsed symbols section")

            foreground_data = parseColorSection(lines, size_data, "foreground color")
            if foreground_data is None:
                continue

            print("parsed foreground color section")

            background_data = parseColorSection(lines, size_data, "background color")
            if background_data is None:
                continue

            print("parsed background color section")

        except StopIteration as e:
            print("the file is missing parts or whole sections, skipping this file")
            continue

    cart_path = path.stem + ".cart"

    print(f"writing file to {cart_path}")

    with open(cart_path, 'wb') as cart_out:
        for size in size_data:
            cart_out.write(size.to_bytes(4, 'little'))

        for row in symbols_data:
            for symbol in row:
                if type(symbol) is str:
                    cart_out.write(symbol.encode('utf-8'))
                else:
                    cart_out.write(symbol.to_bytes(1, 'little'))

        for row in foreground_data:
            for color in row:
                cart_out.write(color.to_bytes(1, 'little'))

        for row in background_data:
            for color in row:
                cart_out.write(color.to_bytes(1, 'little'))
