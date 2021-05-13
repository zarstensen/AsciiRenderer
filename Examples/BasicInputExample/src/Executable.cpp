#include <Asciir.h>
using namespace Asciir;

class ExampleLayer : public Asciir::Layer
{
	Asciir::TermVert pos = { 10, 10 };

	void onUpdate() final
	{
		AREngine::getEngine()->getTerminal()->getRenderer()->clearTerminal();

		if (Input::isKeyDown(Key::W) || Input::isKeyDown(Key::UP))
			pos.y--;

		if (Input::isKeyDown(Key::S) || Input::isKeyDown(Key::DOWN))
			pos.y++;

		if (Input::isKeyDown(Key::A) || Input::isKeyDown(Key::LEFT))
			pos.x--;

		if (Input::isKeyDown(Key::D) || Input::isKeyDown(Key::RIGHT))
			pos.x++;

		TerminalRender* renderer = AREngine::getEngine()->getTerminal()->getRenderer();

		renderer->symbol((char)219);

		renderer->color({0, 0, 155});
		renderer->backgroundColor({ 0, 0, 155 });
		renderer->drawVertices({ {0, 0}, {0, renderer->drawSize().y - 1}, {renderer->drawSize().x - 1, renderer->drawSize().y - 1}, {renderer->drawSize().x - 1, 0} }, DrawMode::Filled);

		renderer->symbol((char)219);
		renderer->color({ (u_char)rand(), (u_char)rand(), (u_char)rand() });
		renderer->drawTile(pos);

		sleep(1000 / 60);
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

Asciir::AREngine* Asciir::CreateEngine(std::vector<std::string> args)
{
	return new Exec;
}
