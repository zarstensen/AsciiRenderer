#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{
public:
	TriangleLayer() = default;

	void onStart() final
	{
		Asciir::Renderer::clear(Asciir::Tile(Asciir::BLUE8));
	}

	void onUpdate(Asciir::DeltaTime delta_time) final
	{
	}
	
	void onResize(Asciir::TerminalResizedEvent& evt)
	{
		Asciir::Texture2D old_screen = Asciir::Renderer::grabScreen();

		old_screen.resizeFill(evt.getSize(), Asciir::Tile(Asciir::BLUE8));

		AR_INFO(evt.getSize());

		Asciir::Renderer::submitShader(&old_screen);
	}

	void onMousePress(Asciir::MousePressedEvent evt)
	{
		AR_NOTIFY(evt.getPos(), evt.getCursorPos());

		Asciir::Tile tile = Asciir::Renderer::viewTile(evt.getCursorPos());

		Asciir::Colour tmp_colour = tile.background_colour;

		tile.background_colour.red = tmp_colour.blue;
		tile.background_colour.green = tmp_colour.red;
		tile.background_colour.blue = tmp_colour.green;

		Asciir::Renderer::submitTile(evt.getCursorPos(), tile);

	}

	void onEvent(Asciir::Event& evt)
	{
		switch(evt.getType())
		{
			case Asciir::EventType::MousePressed:
				onMousePress((Asciir::MousePressedEvent&)evt);
				break;
			case Asciir::EventType::TerminalResized:
				onResize((Asciir::TerminalResizedEvent&)evt);
				break;
		}
	}

};

class Exec : public Asciir::ARApp
{
public:
	void start(const std::vector<std::string>& args) override
	{
		pushLayer(new TriangleLayer);
	}
};

AR_DEFAULT_ENTRYPOINT(Exec);
