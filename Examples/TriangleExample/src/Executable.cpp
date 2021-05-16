#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{

	Asciir::TermVerts triangle_verts = Asciir::TermVerts({ Asciir::TermVert(20, 40), Asciir::TermVert(80, 40), Asciir::TermVert(50, 10) });

	Asciir::TInt pos = 0;
	bool forward = true;

	void onUpdate() final
	{
		Asciir::TerminalRender* renderer = Asciir::AREngine::getEngine()->getTerminal()->getRenderer();
		
		renderer->clearTerminal(Asciir::Tile{ Asciir::Color(25, 25, 25) });
		
		if (forward)
			pos++;
		else
			pos--;

		if (pos > 140 || pos <= 0)
			forward = !forward;

		renderer->setState(Asciir::Tile{ Asciir::Color(255, 255, 255) });
		renderer->drawVertices(triangle_verts.offset(Asciir::TermVert(pos, 0)), Asciir::DrawMode::Filled);
		renderer->resize({ 240, 63 });

		Asciir::sleep(1000 / 60);
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

struct a
{
	int l_a;
	a() { std::cout << l_a; }
};

struct b
{
	a a_var;
	b() : a_var() {}
};

Asciir::AREngine* Asciir::createEngine(std::vector<std::string> args)
{
	return new Exec;
}
