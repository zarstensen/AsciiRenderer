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
