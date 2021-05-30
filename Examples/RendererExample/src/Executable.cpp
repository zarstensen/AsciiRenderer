#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{

	void onStart() final
	{
		Asciir::Renderer::resize({ 100, 50 });
	}

	void onUpdate() final
	{
		Asciir::Renderer::drawRect({ Asciir::Coord(0, 0), Asciir::Coord(99, 49) }, Asciir::Tile(0, 0, ' '));
		Asciir::Renderer::drawRect({ Asciir::Coord( 5, 5 ), Asciir::Coord( 10, 10 ) }, Asciir::Tile(255, 255, 219));
		Asciir::Renderer::resize({ 100, 50 });
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
