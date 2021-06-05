#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{

	void onStart() final
	{
		Asciir::Renderer::resize({ 100, 50 });
	}

	void onUpdate() final
	{
		Asciir::Renderer::clear();
		Asciir::Renderer::drawPolygon({ Asciir::Coord(-10, 15), Asciir::Coord(10, 20), Asciir::Coord(30, 5) }, Asciir::Tile(255, 0, ' '));
		Asciir::Renderer::drawRect({ {30, 30}, {40, 35} }, Asciir::Tile(255));
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
