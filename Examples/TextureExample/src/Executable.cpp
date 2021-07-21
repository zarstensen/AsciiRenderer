#include <Asciir.h>

using namespace Asciir;

class TextureLayer : public Asciir::Layer
{
	Ref<FileTexture> texture = FileTexture();
	const Path texture_path = "./texture.cart";

	void onAdd() final
	{
		texture->load(texture_path);
		std::cout << sizeof(Asciir::Tile) << '\n';
		std::cout << texture->size() << '\n';
	}

	void onUpdate(DeltaTime delta_time) final
	{
		Transform texture_t;
		texture_t.pos = { 10, 10 };
		texture_t.scale = { 2, 2 };
		texture_t.origin = (Coord) texture->size() / 2;

		Renderer::submitShader(texture, texture_t);
		//Renderer::resize({ 20, 20 });
	}

};

class TextureExample : public Asciir::AREngine
{
public:
	TextureExample()
	{
		PushLayer(new TextureLayer());
	}
};

// tile hold char as variant between pointer and stack verison



Asciir::AREngine* Asciir::createEngine(std::vector<std::string> args)
{
	return new TextureExample();
}
