#include <Asciir.h>

using namespace Asciir::AsciirLiterals;

class TriangleLayer : public Asciir::Layer
{

	Asciir::Mesh triangle_verts = Asciir::Mesh(
		{ { {20, 40}, {50, 10}, {80, 40} },
		  { {35, 25}, {65, 25}, {50, 40} } });
	
	Asciir::Real pos = 0;
	bool forward = true;

	Asciir::Real total_time = 0;
	size_t frame_count = 0;

	void onStart() final
	{
		Asciir::Renderer::resize(Asciir::ARApp::getApplication()->getTermRenderer().maxSize());
		Asciir::Renderer::setMinDT(Asciir::DeltaTime(60).fps());
	}

	void onUpdate(Asciir::DeltaTime delta_time) final
	{
		if (frame_count != 0)
			total_time += delta_time.fps();

		frame_count++;

		AR_INFO(total_time / (frame_count - 1));

		Asciir::Renderer::clear(Asciir::Tile(25));

		if (forward)
			pos += 20 * delta_time;
		else
			pos -= 20 * delta_time;

		if (pos > 90 || pos <= 0)
			forward = !forward;

		Asciir::Transform transform;

		transform.setPos(Asciir::Coord(pos, 20));
		transform.setScale(Asciir::Coord(1, 1));
		transform.setOrigin(triangle_verts.getMedianVert());
		transform.setRotation(Asciir::degToRad(pos * 2 + 45_R));

		Asciir::Colour triangle_colour = Asciir::YELLOW8;

		Asciir::Renderer::resize({200, 100});
		Asciir::Renderer::submit(triangle_verts, Asciir::Tile(triangle_colour), transform);

		Asciir::Renderer::setTitle(std::to_string(delta_time.fps()));
	}
	
	void onRemove() final
	{
		AR_INFO("Removed layer from application");
	}

};

class Exec : public Asciir::ARApp
{
public:
	void start(const std::vector<std::string>& args) override
	{
		pushLayer(new TriangleLayer);

		Asciir::Renderer::setFont("Consolas", { 8, 8 });
	}
};

AR_DEFAULT_ENTRYPOINT(Exec)
