
#include <Asciir.h>

using namespace Asciir;
// _R
using namespace Asciir::AsciirLiterals;

class MyLayer : public Layer
{
public:
    // Real = datatype for representing floating point numbers.
    // should be used instead of float or double.
    // _R makes sure the literal is of the Real data type.
    Real speed = 100_R;
    // Coord a 2D vector using Real as its underlying data type
    Coord cube_pos = { 0, 0 };
    // Size2D a 2D vector using size_t as its underlying data type
    Size2D cube_size = { 10, 10 };
    // Colour represents a 4 byte RGBA value
    Colour cube_colour = IBLUE8;

    // on update gets called every frame, and should handle any game / application logic as well as submitting any Graphical structures for rendering, using Renderer::submit. (will be replaced with an Renderer system in the future)
    virtual void onUpdate(DeltaTime dt) override
    {
        // modify the position, depending on which keys are currently pressed.

        // use the Input static class to access the user Input interface.
        if (Input::isKeyDown(Key::LEFT))
            cube_pos.x -= speed * dt;

        if (Input::isKeyDown(Key::RIGHT))
            cube_pos.x += speed * dt;

        if (Input::isKeyDown(Key::UP))
            cube_pos.y -= speed * dt;

        if (Input::isKeyDown(Key::DOWN))
            cube_pos.y += speed * dt;

        // use the submitRect helper function to quickly subit a rectangle with 2 known corners.
        Renderer::submitRect(s_Coords<2>({ cube_pos, cube_pos + (Coord)cube_size }), Tile(cube_colour));
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

        // make sure the Renderer uses multiple threads when rendering frames.
        Renderer::setThreads();
        // set the title of the terminal
        Renderer::setTitle("My Cool Terminal App");
        // limit the framerate to 60 fps.
        Renderer::setMinDT(DeltaTime(60).fps());
        // fix aspect ratio
        Renderer::setFontSize({ 8, 8 });
    }
};

// you can write your own main function and use AsciiInit inside it as an alternative to this
AR_DEFAULT_ENTRYPOINT(MyApplication)