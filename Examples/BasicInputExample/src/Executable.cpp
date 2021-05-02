#include <Asciir.h>
using namespace Asciir;

class ExampleLayer : public Asciir::Layer
{
	Asciir::Coord pos = { 1, 0 };

	void onUpdate() final
	{
		if (Input::isKeyDown(Key::W) || Input::isKeyDown(Key::UP))
			pos.y--;

		if (Input::isKeyDown(Key::S) || Input::isKeyDown(Key::DOWN))
			pos.y++;

		if (Input::isKeyDown(Key::A) || Input::isKeyDown(Key::LEFT))
			pos.x--;

		if (Input::isKeyDown(Key::D) || Input::isKeyDown(Key::RIGHT))
			pos.x++;

		AREngine::getEngine()->getTerminal()->getRenderer()->color({255, 0, 0});
		AREngine::getEngine()->getTerminal()->getRenderer()->symbol(219);
		AREngine::getEngine()->getTerminal()->getRenderer()->drawTile(pos);

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

