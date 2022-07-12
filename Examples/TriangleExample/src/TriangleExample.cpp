#include <Asciir.h>

using namespace Asciir;
using namespace Asciir::AsciirLiterals;

class TriangleLayer : public Layer
{

	// the mesh is constructed by a large triangle (1st row) and a triangle shaped hole (2nd row)
	Mesh triangle_verts = Mesh(
		{ { {20, 40}, {50, 10}, {80, 40} },
		  { {35, 25}, {65, 25}, {50, 40} } });
	
	Real pos = 0;
	bool forward = true;

	Real total_time = 0;
	size_t frame_count = 0;

	void onStart() final
	{
		Renderer::setMinDT(DeltaTime(60).fps());
	}

	void onUpdate(DeltaTime delta_time) final
	{
		// calculate the avrage fps, by using the total runtime and total number of frames rendered
		if (frame_count != 0)
			total_time += delta_time.fps();

		frame_count++;

		// draw the background
		Renderer::clear(Tile(25));


		if (forward)
			pos += 20 * delta_time;
		else
			pos -= 20 * delta_time;

		if (pos > 90 || pos <= 0)
			forward = !forward;

		// setup the transform of the triangle mesh
		Transform transform;

		transform.setPos(Coord(pos, 20));
		transform.setScale(Coord(1, 1));
		// as rotation is used, the origin has to be set. I this case it is set to the centre of the mesh.
		transform.setOrigin(triangle_verts.getMedianVert());
		transform.setRotation(degToRad(pos * 2 + 45_R));

		Colour triangle_colour = YELLOW8;

		Renderer::resize({200, 100});
		Renderer::submit(triangle_verts, Tile(triangle_colour), transform);

		// display the title as [avg fps]:[fps]
		Renderer::setTitle(std::to_string(total_time / (frame_count + 1)) + "\t:\t" + std::to_string(delta_time.fps()));
	}
	
	void onRemove() final
	{
		AR_INFO("Removed layer from application");
	}

};

class Exec : public ARApp
{
public:
	void start(const std::vector<std::string>& args) override
	{
		pushLayer(new TriangleLayer);

		// make sure a character has a 1:1 aspect ratio.
		Renderer::setFontSize({ 8, 8 });
	}
};

AR_DEFAULT_ENTRYPOINT(Exec)
