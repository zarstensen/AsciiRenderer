#include <Asciir.h>

class ExampleLayer : public Asciir::Layer
{
	Asciir::TermVert pos = { 10, 10 };

	void onUpdate() final
	{
		Asciir::Renderer::clear(Asciir::Tile({ 0, 0, 155 }, {0, 0, 155}, 219));

		if (Asciir::Input::isKeyDown(Asciir::Key::W) || Asciir::Input::isKeyDown(Asciir::Key::UP))
			pos.y--;

		if (Asciir::Input::isKeyDown(Asciir::Key::S) || Asciir::Input::isKeyDown(Asciir::Key::DOWN))
			pos.y++;

		if (Asciir::Input::isKeyDown(Asciir::Key::A) || Asciir::Input::isKeyDown(Asciir::Key::LEFT))
			pos.x--;

		if (Asciir::Input::isKeyDown(Asciir::Key::D) || Asciir::Input::isKeyDown(Asciir::Key::RIGHT))
			pos.x++;

		Asciir::Renderer::drawTile(pos, Asciir::Tile(0, { (u_char)rand(), (u_char)rand(), (u_char)rand() }, 219));

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
