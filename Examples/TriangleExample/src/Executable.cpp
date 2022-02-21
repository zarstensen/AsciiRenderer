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
			pos++;
		else
			pos--;

		if (pos > 180 || pos <= 0)
			forward = !forward;

		Asciir::Transform transform;
		Asciir::Transform square_transform;

		square_transform.setPos(Asciir::Coord(pos + 30, 20));
		square_transform.setOrigin(square_mesh->getMedianVert());
		Asciir::Color square_color = Asciir::IRED8;
		square_color.alpha = 100;

		transform.setPos(Asciir::Coord(pos, 0));
		transform.setScale(Asciir::Coord(1, 1));
		transform.setOrigin(triangle_verts->getMedianVert());
		transform.setRotation(Asciir::degToRad(pos + 45));

		Asciir::Color triangle_color = Asciir::YELLOW8;

		Asciir::Renderer::submitMesh(triangle_verts, Asciir::Tile(triangle_color), transform);
		//Asciir::Renderer::submitMesh(square_mesh, Asciir::Tile(square_color), square_transform);

		int a = 2;
		int b = 6;

		AR_CORE_NOTIFY(a, b);

		Asciir::Renderer::resize(Asciir::ARApp::getApplication()->getTermRenderer().maxSize());
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

		/*CONSOLE_FONT_INFOEX font_info;
		font_info.cbSize = sizeof(font_info);

		GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font_info);
		
		font_info.dwFontSize = COORD{ 1, 1};

		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font_info);*/

		pushLayer(new TriangleLayer);
	}
};

AR_DEFAULT_ENTRYPOINT(Exec)
