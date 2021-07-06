#include <Asciir.h>

using namespace Asciir;

class TextureLayer : public Asciir::Layer
{
	Asciir::FileTexture texture;
	const Asciir::Path texture_path = "./texture.cart";

	void onAdd() final
	{
		UTF8Char u8char = "";

		int val = u8char[0] >> 4;

		std::cout << (int)(unsigned char)u8char[0] << '\n';
		std::cout << U8Len(u8"╚╚𒀄𒀚 𒊹");

		texture.load(texture_path);
		std::cout << sizeof(Asciir::Tile) << '\n';
		std::cout << texture.size() << '\n';
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
