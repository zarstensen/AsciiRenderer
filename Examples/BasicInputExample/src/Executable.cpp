#include <Asciir.h>

using namespace Asciir::AsciirLiterals;

class ExampleLayer : public Asciir::Layer
{
	Asciir::Coord pos = { 10, 10 }; // initial position of the box
	static constexpr Asciir::Real speed = 0.025_R; // maximum speed of the box

	// gets called when the application is started, and if this layer already exists on the layer stack.
	// this acts as the layers entry point.
	void onStart()
	{
		// caps the framerate at 60 fps
		Asciir::Renderer::setMinDT(Asciir::DeltaTime(60).fps());
	}

	// gets called every time the application has finished rendering a frame, and needs the layer to submit the next render data.
	// this acts as the main loop for the layer.
	void onUpdate(Asciir::DeltaTime delta_time /*time difference between the last updateand current update*/)
	{
		// start by clearing the background with a dark blue background colour
		Asciir::Renderer::clear(Asciir::Tile(Asciir::BLUE8));

		// use the Asciir::Input namespace to check for user input from the keyboard

		if (Asciir::Input::isKeyDown(Asciir::Key::W) || Asciir::Input::isKeyDown(Asciir::Key::UP))
			// speed is multiplied by the delta_time in order to make the velocity consistent, no matter the framerate
			pos.y -= delta_time.milliSeconds() * speed;

		if (Asciir::Input::isKeyDown(Asciir::Key::S) || Asciir::Input::isKeyDown(Asciir::Key::DOWN))
			pos.y += delta_time.milliSeconds() * speed;

		if (Asciir::Input::isKeyDown(Asciir::Key::A) || Asciir::Input::isKeyDown(Asciir::Key::LEFT))
			pos.x -= delta_time.milliSeconds() * speed;

		if (Asciir::Input::isKeyDown(Asciir::Key::D) || Asciir::Input::isKeyDown(Asciir::Key::RIGHT))
			pos.x += delta_time.milliSeconds() * speed;

		// finally submit a box character with a random foreground colour (for no paticular reason), at the newly calculated position.
		// 9608 = decimal code for a full box symbol. see https://www.w3schools.com/charsets/ref_utf_block.asp for a full list.
		Asciir::Renderer::submit(pos, Asciir::Tile(0, { (u_char)rand(), (u_char)rand(), (u_char)rand() }, Asciir::UTF8Char::fromCode(9608)));
	}
};

class Exec : public Asciir::ARApp
{
public:
	// the start method gets called when the application is initialized, this acts as the entry point for the application.
	void start(const std::vector<std::string>& args)
	{
		// in here, all the necessary layers are pushed onto the layer stack.
		// if the layer is pushed onto the layerstack inside the start method, the onStart method will be called inside the layer.
		pushLayer(new ExampleLayer);
	}
};

// Set the application class that should be instanced
AR_DEFAULT_ENTRYPOINT(Exec)