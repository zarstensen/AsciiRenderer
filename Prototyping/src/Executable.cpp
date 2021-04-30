#include <Asciir.h>
using namespace Asciir;

class ExampleLayer : public Asciir::Layer
{
	Asciir::Coord pos = { 1, 0 };

	void onUpdate() final
	{
		if (Input::isKeyDown(Key::W))
			pos.y--;

		if (Input::isKeyDown(Key::S))
			pos.y++;

		if (Input::isKeyDown(Key::A))
			pos.x--;

		if (Input::isKeyDown(Key::D))
			pos.x++;

		AREngine::getEngine()->getTerminal()->getRenderer()->color({255, 0, 0
	});
		AREngine::getEngine()->getTerminal()->getRenderer()->symbol(219);
		AREngine::getEngine()->getTerminal()->getRenderer()->drawTile(pos);
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

using namespace Asciir;

Asciir::AREngine* Asciir::CreateEngine(std::vector<std::string> args)
{
	return new Exec;
}

