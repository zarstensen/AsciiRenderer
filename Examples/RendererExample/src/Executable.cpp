#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{
	void onStart() final
	{
		Asciir::Renderer::resize({ 100, 50 });
	}

	void onUpdate(Asciir::DeltaTime delta_time) final
	{
		Asciir::Renderer::clear();
		Asciir::Renderer::resize({ 100, 50 });
	}
};

class Exec : public Asciir::ARApp
{
public:
	void start(const std::vector<std::string>& args)
	{
		pushLayer(new TriangleLayer);
	}
};

AR_DEFAULT_ENTRYPOINT(Exec)
