#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{

	Asciir::TermVerts triangle_verts = Asciir::TermVerts({ Asciir::TermVert(20, 40), Asciir::TermVert(80, 40), Asciir::TermVert(50, 10) });

	Asciir::TInt pos = 0;
	bool forward = true;
	Asciir::TerminalRender* renderer;

	void onStart() final
	{
		renderer = Asciir::AREngine::getEngine()->getTerminal()->getRenderer();

		renderer->clearTerminal(Asciir::Tile(Asciir::Color(0, 0, 255)));
	}

	void onUpdate() final
	{
		if (Asciir::Input::isMouseDown(Asciir::MouseKey::LEFT_BUTTON))
		{
			Asciir::MousePressedEvent e = std::get<Asciir::MousePressedEvent>(Asciir::Input::getMouseKeyEvent(Asciir::MouseKey::LEFT_BUTTON));
			if (e.getCursorPos().y > 0)
			{
				Asciir::Tile tile = renderer->getTile(e.getCursorPos());

				Asciir::Color tmp_color = tile.background_color;

				tile.background_color.red = tmp_color.blue;
				tile.background_color.green = tmp_color.red;
				tile.background_color.blue = tmp_color.green;

				renderer->setState(tile);
				renderer->drawTile(e.getCursorPos());
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
