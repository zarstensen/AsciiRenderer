#include <Asciir.h>

class TriangleLayer : public Asciir::Layer
{
	void onStart() final
	{
		Asciir::Renderer::resize({ 5, 5 });
	}

	size_t x_off = 0;

	void onUpdate(Asciir::DeltaTime delta_time) final
	{
		Asciir::Renderer::clear();

		Asciir::Mesh quad({ {0, 0}, {3, 0}, {3, 3}, {0, 3} });

		Asciir::Transform t;

		t.setPos({ 1, 0 });

		if(++x_off % 2)
			Asciir::Renderer::submit(quad, Asciir::IYELLOW8, t);
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
