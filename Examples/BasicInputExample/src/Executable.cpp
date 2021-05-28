#include <Asciir.h>

class ExampleLayer : public Asciir::Layer
{
	Asciir::TermVert pos = { 10, 10 };

	void onUpdate() final
	{
		Asciir::AREngine::getEngine()->getTerminal().getRenderer()->clearTerminal();

		if (Asciir::Input::isKeyDown(Asciir::Key::W) || Asciir::Input::isKeyDown(Asciir::Key::UP))
			pos.y--;

		if (Asciir::Input::isKeyDown(Asciir::Key::S) || Asciir::Input::isKeyDown(Asciir::Key::DOWN))
			pos.y++;

		if (Asciir::Input::isKeyDown(Asciir::Key::A) || Asciir::Input::isKeyDown(Asciir::Key::LEFT))
			pos.x--;

		if (Asciir::Input::isKeyDown(Asciir::Key::D) || Asciir::Input::isKeyDown(Asciir::Key::RIGHT))
			pos.x++;

		Asciir::TerminalRender* renderer = Asciir::AREngine::getEngine()->getTerminal().getRenderer();

		renderer->symbol((char)219);

		renderer->color({0, 0, 155});
		renderer->backgroundColor({ 0, 0, 155 });
		renderer->drawVertices({ Asciir::TermVert(0, 0), Asciir::TermVert(0, renderer->drawSize().y - 1), Asciir::TermVert(renderer->drawSize().x - 1, renderer->drawSize().y - 1), Asciir::TermVert(renderer->drawSize().x - 1, 0) }, Asciir::DrawMode::Filled);

		renderer->symbol((char)219);
		renderer->color({ (u_char)rand(), (u_char)rand(), (u_char)rand() });
		renderer->drawTile(pos);

		Asciir::sleep(1000 / 60);
	}
};

class Exec : public Asciir::AREngine
{
public:
	Exec()
	{
		PushLayer(new ExampleLayer);
	}

	~Exec() {}
};

Asciir::AREngine* Asciir::createEngine(std::vector<std::string> args)
{
	return new Exec;
}
