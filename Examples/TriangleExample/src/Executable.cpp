#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{

	Asciir::TermVerts triangle_verts = Asciir::TermVerts({ Asciir::TermVert(20, 40), Asciir::TermVert(80, 40), Asciir::TermVert(50, 10) });

	Asciir::TInt pos = 0;
	bool forward = true;

	void onStart() final
	{
		Asciir::Renderer::resize({ 240, 63 });
	}

	void onUpdate() final
	{
		
		Asciir::Renderer::clear(Asciir::Tile(25));
		
		if (forward)
			pos++;
		else
			pos--;

		if (pos > 180 || pos <= -80)
			forward = !forward;

		Asciir::Renderer::drawPolygon(triangle_verts.offset(Asciir::TermVert(pos, 0)), Asciir::Tile(255));
		
		Asciir::Renderer::resize({ 240, 63 });

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

Asciir::AREngine* Asciir::createEngine(std::vector<std::string> args)
{
	return new Exec;
}
