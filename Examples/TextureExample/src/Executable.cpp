#include <Asciir.h>

using namespace Asciir;

class TileShader : public Shader2D
{

	Size2D size() const final override
	{
		return Renderer::size();
	}

	Tile readTile(const Size2D& coord, const DeltaTime&, const size_t& frame) const final override
	{
		if((coord.x + coord.y + frame) % 2)
			return Tile(BLACK8);
		else
			return Tile(IWHITE8);
	}
};

class TextureLayer : public Asciir::Layer
{
	Ref<FileTexture> texture = FileTexture();
	Ref<TileShader> shader = TileShader();
	const Path texture_path = "./test.cart";

	void onAdd() final
	{
		//texture->load(texture_path);
		std::cout << sizeof(Asciir::Tile) << '\n';
		std::cout << texture->size() << '\n';
	}

	void onUpdate(DeltaTime delta_time) final
	{
		Transform texture_t;
		texture_t.pos = { 10, 10 };
		texture_t.scale = { 1, 1 };
		texture_t.origin = (Coord)texture->size() / 2;

		Renderer::submitShader(shader);
		//Renderer::submitShader(texture, texture_t);
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