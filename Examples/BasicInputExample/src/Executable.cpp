#include <Asciir.h>

class ExampleLayer : public Asciir::Layer
{
	Asciir::Coord pos = { 10, 10 };
	static constexpr Asciir::Real speed = 0.025;

	void onStart()
	{
		Asciir::Renderer::setMinDT(Asciir::DeltaTime::FPS(60));
	}

	void onUpdate(Asciir::DeltaTime delta_time) final
	{
		Asciir::Renderer::clear(Asciir::Tile({ 0, 0, 155 }, { 0, 0, 155 }, 219));

		if (Asciir::Input::isKeyDown(Asciir::Key::W) || Asciir::Input::isKeyDown(Asciir::Key::UP))
			pos.y -= delta_time * speed;

		if (Asciir::Input::isKeyDown(Asciir::Key::S) || Asciir::Input::isKeyDown(Asciir::Key::DOWN))
			pos.y += delta_time * speed;

		if (Asciir::Input::isKeyDown(Asciir::Key::A) || Asciir::Input::isKeyDown(Asciir::Key::LEFT))
			pos.x -= delta_time * speed;

		if (Asciir::Input::isKeyDown(Asciir::Key::D) || Asciir::Input::isKeyDown(Asciir::Key::RIGHT))
			pos.x += delta_time * speed;

		Asciir::Renderer::submitTile(pos, Asciir::Tile(0, { (u_char)rand(), (u_char)rand(), (u_char)rand() }, 219));
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