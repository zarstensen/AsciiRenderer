#include <Asciir.h>

using namespace Asciir::AsciirLiterals;

class TriangleLayer : public Asciir::Layer
{
	Asciir::Ref<Asciir::Mesh> triangle_verts = Asciir::Mesh(
		{ { {20, 40}, {50, 10}, {80, 40} },
		  { {35, 25}, {65, 25}, {50, 40} } });

	Asciir::Ref<Asciir::Mesh> square_mesh = Asciir::Mesh({ {0, 0}, { 15, 0 }, {15, 15}, {0, 15} });

	Asciir::TInt pos = 0;
	bool forward = true;

	Asciir::Real total_time = 0;
	size_t frame_count = 0;

	void onStart() final
	{
		Asciir::Renderer::resize({ 240, 63 });
		Asciir::Renderer::setMinDT(Asciir::DeltaTime::FPS(60));
	}

	void onUpdate(Asciir::DeltaTime delta_time) final
	{
		if (frame_count != 0)
			total_time += delta_time.fps();
		frame_count++;
		AR_INFO(total_time / (frame_count - 1));

		Asciir::Renderer::clear(Asciir::Tile(25));

		if (forward)
			pos++;
		else
			pos--;

		if (pos > 180 || pos <= 0)
			forward = !forward;

		Asciir::Transform transform;
		Asciir::Transform square_transform;

		square_transform.pos.x = pos + 30;
		square_transform.pos.y = 20;
		square_transform.origin = square_mesh->getMedianVert();
		Asciir::Color square_color = Asciir::IRED8;
		square_color.alpha = 100;

		transform.pos.x = pos;
		transform.pos.y = 0;
		transform.scale.x = 1;
		transform.origin = triangle_verts->getMedianVert();
		transform.rotation = Asciir::degToRad(pos + 45);

		Asciir::Color triangle_color = Asciir::YELLOW8;

		Asciir::Renderer::submitMesh(triangle_verts, Asciir::Tile(triangle_color), transform);
		Asciir::Renderer::submitMesh(square_mesh, Asciir::Tile(square_color), square_transform);

		Asciir::Renderer::resize({ 240, 63 });
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