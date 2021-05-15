#include <Asciir.h>

class TriangleLayer: public Asciir::Layer
{
	void onUpdate() final
	{
		Asciir::TerminalRender* renderer = Asciir::AREngine::getEngine()->getTerminal()->getRenderer();

		renderer->clearTerminal(Asciir::Tile{ Asciir::Color(25, 25, 25) });

		Asciir::TermVerts triangle_verts({ {20, 40}, {80, 40}, { 50, 10} });

		renderer->setState(Asciir::Tile{ Asciir::Color(255, 255, 255) });
		renderer->drawVertices(triangle_verts, Asciir::DrawMode::Filled);
		renderer->resize({ 100, 50 });

	}
};

class Exec: public Asciir::AREngine
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
