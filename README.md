<!--
*** Inspired by Best-README-Template (https://github.com/othneildrew/Best-README-Template).
-->

<div id="top"></div>


![image][AsciirBanner]
> ###### [this banner graphic](#asciirbanner) was rendered and displayed inside a Windows Command Prompt using Asciir.
![GitHub Actions CI](https://github.com/karstensensensen/AsciiRenderer/actions/workflows/windows_cmake.yml/badge.svg "GitHub Actions CI")

- [Ascii Renderer](#ascii-renderer)
  - [Branch Details](#branch-details)
  - [Examples (More coming soon)](#examples-more-coming-soon)
    - [TriangleExample](#triangleexample)
    - [AsciirBanner](#asciirbanner)
  - [Built With](#built-with)
  - [Getting Started](#getting-started)
    - [Building](#building)
    - [Link using cmake.](#link-using-cmake)
  - [Usage](#usage)
  - [CMake](#cmake)
    - [ASCIIR_LOG_VIEWER](#asciir_log_viewer)
    - [ASCIIR_EXAMPLES](#asciir_examples)
    - [ASCIIR_HIGH_PRECISSION_FLOAT](#asciir_high_precission_float)
    - [ASCIIR_AUTO_ISNTALL_DEPS](#asciir_auto_isntall_deps)
  - [Performance](#performance)
  - [Roadmap](#roadmap)
    - [near future](#near-future)
    - [far future](#far-future)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

# Ascii Renderer

The Ascii Renderer project, Asciir for short, is a Renderer / Game engine that uses the terminal / console as its window. It makes use of ANSI control sequences in order to control the terminal.

Displaying graphics in a terminal can often be very tedious and inconvenient, as it was not really designed for it. This is what Asciir aims to eliminate, by providing a library that allows the user to render graphics in the terminal with ease. It allows one to easily display graphics in the terminal as well as control the terminal window size (depending on the terminal), title and other properties. As Asciir also is intended to be used as a game engine, it also aims to provide the highest frame rate possible, for the given task.

## Branch Details

This branch integrates the fully developed Entity Component System into the Asciir workflow.

## Examples (More coming soon)

here are some demo projects build with Asciir, see the examples folder for more examples.

### TriangleExample

simply displays three yellow triangles that move and rotate on the screen. The framerate is displayed as the title.  
![image][TriangleExampleGif]

### AsciirBanner
This example projects contains all the code that was used to generate the banner graphic displayed at the top of the README.
It makes use of texture loading and more complex shaders for the lightning in the background and some slight enhancements to loaded textures.

<p align="right">(<a href="#top">back to top</a>)</p>


## Built With

- 3rd party
  - [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) (linear algebra)
  - [CImg](https://github.com/dtschump/CImg) (loading images)
  - [zlib](https://github.com/madler/zlib) (uncompressing files)
  - [SFML](https://www.sfml-dev.org/) (audio)
  - [FastNoise2](https://github.com/Auburn/FastNoise2/) (noise generation)
  - [libpng](https://github.com/glennrp/libpng) (reading png files)
  - [libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo) (reading jpg files)

- My projects
  - [EThread](https://github.com/karstensensensen/EThread)
  - [ChrTrcProfiler](https://github.com/karstensensensen/ChrTrcProfiler) (visual profiling)

<p align="right">(<a href="#top">back to top</a>)</p>

## Getting Started

Currently, Asciir only works on the Windows platform, however cross platform support is planned to be implemented some time in the future.

if using ASCIIR_CONAN_AUTO_INSTALL, conan is required
* Conan
  ```
  pip install conan
  ```


### Building
Asciir uses CMake as its build system and conan to handle the download of some dependencies.

1. Clone the Asciir repositry (remember --recursive)

    ``` bat
    git clone --recursive https://github.com/karstensensensen/AsciiRenderer
    ```

2. cd into the cloned repositry in a terminal

    ``` bat
    cd Asciir
    ```

3. Configure cmake *(omit -DASCIIR_AUTO_INSTALL_DEPS if conan should not be used)*

   ``` bat
   mkdir build
   cmake -DASCIIR_AUTO_INSTALL_DEPS=ON -S . -B ./build 
   ```

4. build the library (this may take a while the first time)

   ``` bat
   cmake --build ./build
   ```

now you can link your project to the build static library. As mentioned earlier, a OpenAL dll is required if audio is used at any point.

### Link using cmake.

Asciir exposes a target inside CMake any external projects can link to, in order to use Asciir in their build system.

1. Start by adding Asciir as a submodule inside your project folder structure
   ```
   git submodule add https://github.com/karstensensensen/AsciiRenderer
   ```
2. add the following lines to your CMakeLists.txt
   ```CMake
   ...
   add_subdirectory("${CMAKE_CURRENT_SRC_DIR}/.../Asciir")
   
   target_link_libraries(${PROJECT_NAME} Asciir::Asciir)
   ...
   ```

now your project should be able to include the Asciir headers as well as link to the Asciir libraries. For more details see the [CMake](#cmake) section.

<p align="right">(<a href="#top">back to top</a>)</p>



## Usage

Each Asciir project starts by creating an ARApp that pushes at least one Layer onto its layerstack. Inside this Layer, functions like onAdd, onRemove and onUpdate can be overridden in order to implement application logic.

``` cpp
#include <Asciir.h>

using namespace Asciir;
// _R
using namespace Asciir::AsciirLiterals;

class MyLayer : public Layer
{
public:
    // on update gets called every frame, and should handle any game / application logic as well as submitting any Graphical structures for rendering, using Renderer::submit. (will be replaced with an Renderer system in the future)
    virtual void onUpdate(DeltaTime dt) override
    {

    }

};

class MyApplication : public ARApp
{
public:
    // Called once when the Asciir library has been initialized and is ready to start.
    // args contains the command line arguments.
    virtual void start(const std::vector<std::string>& args) override
    {
        // push MyLayer onto the layer stack.
        pushLayer(new MyLayer());
    }
};

// you can write your own main function and use AsciiInit inside it as an alternative to this
AR_DEFAULT_ENTRYPOINT(MyApplication)
```

Now we can set up the renderer as well as set the title of our application.
This is done through the Renderer interface.
``` cpp
...
class MyApplication : public ARApp
{
public:
    // Called once when the Asciir library has been initialized and is ready to start.
    // args contains the command line arguments.
    virtual void start(const std::vector<std::string>& args) override
    {
        // push MyLayer onto the layer stack.
        pushLayer(new MyLayer());
        
        // make sure the Renderer uses multiple threads when rendering frames.
        Renderer::setThreads();
        // set the title of the terminal
        Renderer::setTitle("My Cool Terminal App");
        // limit the framerate to 60 fps.
        Renderer::setMinDT(DeltaTime(60).fps());
    }
};
...
```

Now our application just need to actually do something. In this example we will create a blue box that can be controlled by the arrow keys on the keyboard. So lets start by adding some variables.

``` cpp

class MyLayer : public Layer
{
public:
    ...
    // the speed of the cube when moving.
    // Real = datatype for representing floating point numbers.
    // should be used instead of float or double.
    // _R makes sure the literal is of the Real data type.
    Real speed = 100_R;
    // the position of the cube on the screen, 1 unit = 1 terminal character
    // Coord a 2D vector using Real as its underlying data type
    Coord cube_pos = { 0, 0 };
    // the width and height of the cube, 1 unit = 1 terminal character
    // Size2D a 2D vector using size_t as its underlying data type
    Size2D cube_size = { 10, 10 };
    // the colour of the box. Here we use a constant provided by the Asciir library for a standard blue colour in a terminal.
    // Colour represents a 4 byte RGBA value
    Colour cube_colour = IBLUE8;
    ...
};
```

Finally, we need to move the cube around and render it to the screen. We do this by using the Input interface in order to check for key presses, and the Renderer interface to submit a rectangle to the renderer.

``` cpp
class MyLayer : public Layer
{
    ...
    // on update gets called every frame, and should handle any game / application logic as well as submitting any Graphical structures for rendering, using Renderer::submit. (will be replaced with an Renderer system in the future)
    virtual void onUpdate(DeltaTime dt) override
    {
        // modify the position, depending on which keys are currently pressed.

        // use the Input static class to access the user Input interface.
        // here we multiply by "dt". dt is the time that has passed since the last call to update, so by multiplying by this value, we make sure our velocity is independent of framerate.
        if (Input::isKeyDown(Key::LEFT))
            cube_pos.x -= speed * dt;

        if (Input::isKeyDown(Key::RIGHT))
            cube_pos.x += speed * dt;

        if (Input::isKeyDown(Key::UP))
            cube_pos.y -= speed * dt;

        if (Input::isKeyDown(Key::DOWN))
            cube_pos.y += speed * dt;

        // use the submitRect helper function to quickly submit a rectangle by submitting its top right and bottom left corner, as well as its colour.
        Renderer::submitRect(s_Coords<2>({ cube_pos, cube_pos + (Coord)cube_size }), Tile(cube_colour));
    }
    ...
};
```

The final code should look something like this.
``` cpp
#include <Asciir.h>

using namespace Asciir;
// _R
using namespace Asciir::AsciirLiterals;

class MyLayer : public Layer
{
public:
    Real speed = 100_R;

    Coord cube_pos = { 0, 0 };

    Size2D cube_size = { 10, 10 };

    Colour cube_colour = IBLUE8;

    virtual void onUpdate(DeltaTime dt) override
    {
        if (Input::isKeyDown(Key::LEFT))
            cube_pos.x -= speed * dt;

        if (Input::isKeyDown(Key::RIGHT))
            cube_pos.x += speed * dt;

        if (Input::isKeyDown(Key::UP))
            cube_pos.y -= speed * dt;

        if (Input::isKeyDown(Key::DOWN))
            cube_pos.y += speed * dt;
        
        Renderer::submitRect(s_Coords<2>({ cube_pos, cube_pos + (Coord)cube_size }), Tile(cube_colour));
    }

};

class MyApplication : public ARApp
{
public:
    virtual void start(const std::vector<std::string>& args) override
    {
        pushLayer(new MyLayer());

        Renderer::setThreads();

        Renderer::setTitle("My Cool Terminal App");

        Renderer::setMinDT(DeltaTime(60).fps());
    }
};

// you can write your own main function and use AsciiInit inside it as an alternative to this
AR_DEFAULT_ENTRYPOINT(MyApplication)
```

More tutorials will come in the future, in the meantime checkout the codebase reference to discover some of Asciirs features by yourself!

<p align="right">(<a href="#top">back to top</a>)</p>

## CMake

The following variables are exposed and are initialized with the displayed default values.
```CMake
ASCIIR_LOG_VIEWER = ON
ASCIIR_EXAMPLES = OFF
ASCIIR_HIGH_PRECISSION_FLOAT = OFF
ASCIIR_AUTO_INSTALL_DEPS = OFF
ASCIIR_PARALLEL_BUILD = ON
```

### ASCIIR_LOG_VIEWER

Builds a executable that is able to display Asciir log files.
As a process can only own 1 terminal at a time (on Windows at least), a seperate program is needed for logging, as the primary program is already using the terminal for displaying graphics.


### ASCIIR_EXAMPLES

Builds all the example projects in the examples folder.

### ASCIIR_HIGH_PRECISSION_FLOAT

Use double instead of float for the Real typedef.

### ASCIIR_AUTO_ISNTALL_DEPS

Automatically installs all the dependencies using the Conan package manager. (conan is required for this to work)

The alias target `Asciir::Asciir` can be used if one wants to link to the Asciir library through CMake.

### ASCIIR_PARALLEL_BUILD

Builds the Asciir library using multiple threads.

This should be enabled for a faster build time, at the cost of a higher CPU load.

## Performance

The slowest process during the rendering is by far actually printing the new graphics to the terminal. As the terminal does not erase its contents everyframe, only modified tiles, compared to the previous frame, are actually printed to the terminal. If a high fps is required, this optimization should be kept in mind, as one can still have a relatively complex scene rendered and still have a high frame rate, as long as the modified tiles are kept to a minnimum.  
  
An example of an optimization could be the rendering of an expanding gradient. Event though each tile might only have its colour values changed by relatively small values, they still have to be redrawn. In order to optimize this example, we instead need to reduce the number of tiles changed pr. frame. This can be done by intentianolly introducing banding in the gradient's colours, as this will reduce the number of times a tile will change colour, and thus reduce the number of writes to the terminal.

<p align="right">(<a href="#top">back to top</a>)</p>

## Roadmap

### near future

- [x] Make README more nice
- [ ] Improve documentation + tutorials
- [ ] Add built in Systems and Components (Script system, Renderer system, Physics system...)
- [ ] Cleanup CMake file
- [ ] Implement all possible character attributes for the Tile class
- [ ] Integrate the Entity Component System into the game engine
- [ ] Create a demo game
- [ ] Refactor some of the code base

### far future

- [ ] Implement a tree based entity structure (like godot)
- [ ] Create a Texture editor
- [ ] Cross platform support
- [ ] Graphical User interface tools (widgets and stuff)
- [ ] Create a Game Engine editor

<p align="right">(<a href="#top">back to top</a>)</p>

## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>

## Acknowledgments

This game engine was built with the help of [The Cherno's game engine series](https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT). I highly recommend it as a good starting point for creating your very own game engine!

<!-- References -->
[TriangleExampleGif]: https://github.com/karstensensensen/AsciiRenderer/blob/master/Resources/TextureExampleGif.gif ""
[AsciirBanner]: https://github.com/karstensensensen/AsciiRenderer/blob/master/Resources/AsciirBanner.gif "This banner was rendered in a terminal using the Asciir engine!"
