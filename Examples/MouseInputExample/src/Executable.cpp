#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{
public:
	TriangleLayer() = default;

	void onStart() final
	{
		Asciir::Renderer::clear(Asciir::Tile(Asciir::Color(0, 0, 255)));
	}

	void onUpdate(Asciir::DeltaTime delta_time) final
	{
		if (Asciir::Input::isMouseToggled(Asciir::MouseKey::LEFT_BUTTON))
		{
			Asciir::MousePressedEvent e = std::get<Asciir::MousePressedEvent>(Asciir::Input::getMouseKeyEvent(Asciir::MouseKey::LEFT_BUTTON));
			if (e.getCursorPos().y > 0)
			{
				Asciir::Tile tile = Asciir::Renderer::viewTile(e.getCursorPos());

				Asciir::Color tmp_color = tile.background_color;

				tile.background_color.red = tmp_color.blue;
				tile.background_color.green = tmp_color.red;
				tile.background_color.blue = tmp_color.green;

				
				Asciir::Renderer::submitTile(e.getCursorPos(), tile);
			}
		}
	}
};

class Exec : public Asciir::AREngine
{
public:
	Exec()
	{
		PushLayer(new TriangleLayer);
	}
};

Asciir::AREngine* Asciir::createEngine(std::vector<std::string> args)
{
	return new Exec;
}
