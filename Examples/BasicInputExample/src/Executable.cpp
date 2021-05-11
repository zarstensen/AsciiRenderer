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

		if (pos.y != 0)
		{
			pos.x++;
			pos.y--;
		}

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
	std::cout << "\x1b[48;2;0;0;155;38;2;0;0;155m\x1b[?25l\x1b[1;1f";

	std::cout << "\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\x1b[38;2;155;0;0mAB";
	std::cout << "\x1b[48;2;0;0;155;38;2;0;0;155m\x1b[1;3f\xdb";

	std::cin.get();

	//for (size_t y = 0; y < 50; y++) {
	//	for (size_t x = 0; x < 100; x++)
	//		std::cout << '\xdb';
	//	//std::cout << '\n';
	//}

	//std::cout << "\x1b[1;1f\x1b[10;12f\x1b[;23;24;25;29;38;2;132;225;108;48;2;0;0;155m\xdb";
	//sleep(100);
	//std::cout << "\x1b[1;1f\x1b[9;12f\x1b[;23;24;25;29;38;2;132;225;108;48;2;0;0;155m\xdb\x1b[10;12f\x1b[;38;2;0;0;155m\xdb\x1b[20;20f\xdb\x1b[;38;2;0;0;155m\x1b[10;11f\xdb\x1b[31;31f\x1b[50;100f";
	//sleep(100);
	////std::cout << "\x1b[1;1f\x1b[8;12f\x1b[;23;24;25;29;38;2;214;174;82;48;2;0;0;155m\xdb\x1b[9;12f\x1b[;38;2;0;0;155m\xdb\xdb\x1b[31;31f\x1b[50;100f";
	//std::cin.get();
	
	return new Exec;
}
