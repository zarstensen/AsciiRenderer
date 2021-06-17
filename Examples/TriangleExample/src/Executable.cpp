#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{

	Asciir::Mesh triangle_verts = Asciir::Mesh(
		{ { {20, 40}, {50, 10}, {80, 40} },
		  { {35, 25}, {65, 25}, {50, 40} } });

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
		Asciir::Renderer::drawMesh(triangle_verts.offset({ forward ? 1 : -1, 0 }), Asciir::Tile(Asciir::YELLOW8));

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

std::ostream& operator<<(std::ostream& stream, bool boolean_value)
{
	stream << (boolean_value ? "true" : "false");
	return stream;
}

Asciir::AREngine* Asciir::createEngine(std::vector<std::string> args)
{
	return new Exec;
}
