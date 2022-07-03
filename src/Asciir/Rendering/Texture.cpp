#include "Texture.h"

#include "arpch.h"

#include <ChrTrc.h>

using namespace cimg_library;

namespace Asciir
{
	// ============ Texture2D ============
	
	Texture2D::Texture2D(const Size2D& new_size, const Tile& new_tile)
		: m_texture(new_size)
	{
		m_texture.fill(new_tile);
	}

	// uv, dt, and df is never used here
	Tile Texture2D::readTile(const TermVert& coord, Coord, const DeltaTime&, size_t)
	{
		if (m_tiled_size != TermVert(-1, -1))
		{
			AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Coordinate out of bounds for Texture2D read.\nCoordinate: ", coord,
				"\nTiled size: ", m_tiled_size,
				"\nTexture size: ", m_texture.dim());

			coord.x %= m_texture.width();
			coord.y %= m_texture.height();
		}

		return m_texture(coord);
	}

	void Texture2D::setTile(const Size2D& coord, const Tile& new_tile)
	{
		if (m_tiled_size != TermVert(-1, -1))
		{
			AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Coordinate out of bounds for Texture2D read.\nCoordinate: ", coord,
				"\nTiled size: ", m_tiled_size,
				"\nTexture size: ", m_texture.dim());

			coord.x %= m_texture.width();
			coord.y %= m_texture.height();
		}

		m_texture(coord) = new_tile;
	}

	void Texture2D::blendTile(const Size2D& coord, const Tile& overlay_tile)
	{
		if (m_tiled_size != TermVert(-1, -1))
		{
			AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Coordinate out of bounds for Texture2D read.\nCoordinate: ", coord,
				"\nTiled size: ", m_tiled_size,
				"\nTexture size: ", m_texture.dim());

			coord.x %= m_texture.width();
			coord.y %= m_texture.height();
		}

		m_texture(coord).blend(overlay_tile);
	}

	TermVert Texture2D::size() const
	{
		return m_tiled_size == TermVert(-1, -1) ? textureSize() : m_tiled_size;
	}

	void Texture2D::resize(const Size2D& new_size, RESIZE mode, const Tile& fill_tile)
	{
		// call function depending on mode
		switch (mode)
		{
			case RESIZE::CLEAR:
				resizeClear(new_size, fill_tile);
				break;
			case RESIZE::FILL:
				resizeFill(new_size, fill_tile);
				break;
			case RESIZE::NEAREST:
				resizeNearest(new_size);
				break;
		}
	}

	void Texture2D::resizeClear(const Size2D& new_size, const Tile& fill_tile)
	{
		m_texture.resizeClear(new_size);

		// only fill texture with fill_tile if it is not the default tile value
		if (fill_tile != Tile())
			m_texture.fill(fill_tile);
	}

	void Texture2D::resizeFill(const Size2D& new_size, const Tile& fill_tile)
	{
		Size2D prev_size = size();
		m_texture.resize(new_size);

		if (fill_tile != Tile())
		{
			// fill newly resized part of texture with fill_tile
			if (new_size.y > prev_size.y)
			{
				// top to bottom should fill the most values, as the data should be stored continuously
				m_texture.block(prev_size.y, 0, new_size.y - prev_size.y, new_size.x).fill(fill_tile);
			}

			if (new_size.x > prev_size.x)
			{
				m_texture.block(0, prev_size.x, new_size.y, new_size.x - prev_size.x).fill(fill_tile);
			}
		}
	}

	void Texture2D::resizeNearest(const Size2D& new_size)
	{
		Texture2D new_img(new_size);

		Real ratio_x = m_texture.width() / (Real)new_size.x;
		Real ratio_y = m_texture.height() / (Real)new_size.y;

		for (size_t i = 0; i < new_size.x; i++)
		{
			for (size_t j = 0; j < new_size.y; j++)
			{
				Size2D coord(std::floor(i * ratio_x), std::floor(j * ratio_y));

				new_img.setTile({ i, j }, m_texture(coord));
			}
		}

		*this = std::move(new_img);
	}

	// ============ FileTexture ============
	FileTexture& FileTexture::load(const Path& dir)
	{
		Path abs_dir = std::filesystem::absolute(dir);
		AR_ASSERT_MSG(!m_is_loaded, "cannot load file into an already loaded texture");

		if (std::filesystem::exists(abs_dir))
		{
			Path file_type = m_file_dir.extension();

			if (file_type == ".cart")
				loadCART(abs_dir);
			else if (file_type == ".xp")
				loadXP(abs_dir);
			else if (file_type == ".txt")
				loadTXT(abs_dir);
			else // if none of the above is true, assume the file is an image file
				loadIMG(abs_dir);
		}
		else
		{
			AR_CORE_ERR("the file ", dir, " does not exist, unable to load texture");
			AR_ASSERT(false);
		}

		return *this;
	}

	FileTexture& FileTexture::loadCART(const Path& cart_file)
	{
		std::ifstream texture_in(cart_file, std::ios::binary);

		// check the header
		char header[4];

		texture_in.read(header, 4);

		AR_ASSERT_MSG(strcmp(header, "CART") == 0, "Invalid header in file: ", cart_file);

		// load the size into memory

		size_t width;
		size_t height;

		texture_in.read((char*)&width, sizeof(size_t));
		texture_in.read((char*)&height, sizeof(size_t));

		// allocate memory for the texture

		resizeClear({ width, height }, Tile());

		// load texture into memory
		for(Tile& elem: m_texture.reshaped())
		{
			texture_in.read((char*)elem.symbol, 1);
			texture_in.read((char*)elem.symbol + 1, U8CharSize(elem.symbol) - 1);

			texture_in.read((char*)&elem.colour, sizeof(elem.colour));

			texture_in.read((char*)&elem.background_colour, sizeof(elem.background_colour));
		}

		m_file_dir = cart_file;
		m_is_loaded = true;

		return *this;
	}

	FileTexture& FileTexture::loadXP(const Path& xp_file, size_t layer, const std::array<uint32_t, 0x100>& font_map)
	{
		gzFile xp_in = gzopen(xp_file.string().c_str(), "rb");
		
		// skip version number
		gzseek(xp_in, sizeof(int32_t), SEEK_CUR);

		int32_t layer_count;
		gzread(xp_in, &layer_count, sizeof(int32_t));

		AR_ASSERT_MSG(layer < layer_count, "Layer index of XP file must not be greater than the layer count of the file.");

		int32_t current_layer = 0;

		// skip layers until the target layer is reached
		while (current_layer < layer)
		{
			// these should be the same for every layer, but they are read anyways in case of future changes to the format specification
			int32_t width, height;
			gzread(xp_in, &width, sizeof(int32_t));
			gzread(xp_in, &height, sizeof(int32_t));

			gzseek(xp_in, (sizeof(uint32_t) + sizeof(int8_t) * 6) * width * height, SEEK_CUR);
		}

		// as the data is stored in column major order, there is no reason to keep track of the x y indicies.

		int32_t width, height;
		gzread(xp_in, &width, sizeof(int32_t));
		gzread(xp_in, &height, sizeof(int32_t));

		resize({ width, height }, RESIZE::CLEAR);

		for (uint32_t i = 0; i < (uint32_t)width * (uint32_t)height; i++)
		{
			// get symbol
			uint32_t symbol_seq;
			gzread(xp_in, &symbol_seq, sizeof(uint32_t));

			m_texture[i].symbol = UTF8Char::fromCode(font_map[symbol_seq]);

			// get foreground

			uint8_t c_val[3];

			gzread(xp_in, c_val, sizeof(uint8_t) * 3);

			m_texture[i].colour = Colour(c_val[0], c_val[1], c_val[2]);

			// get background

			gzread(xp_in, c_val, sizeof(uint8_t) * 3);

			m_texture[i].background_colour = Colour(c_val[0], c_val[1], c_val[2]);

			// transparent background
			if (m_texture[i].background_colour == Colour(255, 0, 255))
				m_texture[i].background_colour = Colour(0, 0);
		}

		gzclose(xp_in);

		m_file_dir = xp_file;
		m_is_loaded = true;

		return *this;
	}

	FileTexture& FileTexture::loadTXT(const Path& txt_file)
	{
		std::wifstream file_in(txt_file);
		file_in.imbue(std::locale("en_US.UTF-8"));

		// make sure the txt file is formatted correctly.

		size_t width = 0;
		size_t height = 0;
		size_t curr_w = 0;

		wchar_t c;

		for (file_in.read(&c, 1);!file_in.eof();file_in.read(&c, 1))
		{
			if (c == '\n')
			{
				if (width != 0 && curr_w != width)
				{
					AR_ASSERT_MSG("Text file formatted incorrectly, row width was not consistent, at row: ", height);
					return *this;
				}

				width = curr_w;
				curr_w = 0;
				height++;
			}
			else
			{
				curr_w++;
			}
		}

		if (width != 0 && curr_w != width)
		{
			AR_ASSERT_MSG("Text file formatted incorrectly, row width was not consistent, at row: ", height);
			return *this;
		}

		// last line does not have a newline, so this should be incremented to account for this
		height++;

		// the width and height were found doing the format check, so the entire texture can be allocated at once.

		resize({ width, height }, RESIZE::CLEAR);

		// reset file stream
		file_in.clear();
		file_in.seekg(file_in.beg);

		size_t x = 0;
		size_t y = 0;

		for(file_in.read(&c, 1);!file_in.eof(); file_in.read(&c, 1))
		{
			if (c == '\n')
			{
				y++;
				x = 0;

				// continue here, as the newline itself should not be stored
				continue;
			}

			setTile({ x, y }, Tile(BLACK8, WHITE8, UTF8Char(c)));

			x++;
		}

		m_file_dir = txt_file;
		m_is_loaded = true;

		return *this;
	}

	FileTexture& FileTexture::loadIMG(const Path& img_file, bool use_half_tiles, bool load_foreground)
	{
		CT_MEASURE_N("Image Load");
		AR_ASSERT_VOLATILE(std::filesystem::exists(img_file), "Could not find image file: ", img_file);

		try
		{
			CImg<uint8_t> image_data(img_file.string().c_str());

			if (use_half_tiles)
			{
				resize({ image_data.width(), (image_data.height() + 1) / 2 - 1 }, RESIZE::CLEAR);

				for (uint32_t x = 0; x < (uint32_t)image_data.width(); x++)
				{
					for (uint32_t y = 0; y < (uint32_t)image_data.height() / 2; y++)
					{
						Colour foreground;
						Colour background;

						/// grayscale values
						if (image_data.spectrum() == 1)
						{
							background = Colour(image_data(x, y, 0, 0));
							foreground = Colour(image_data(x, y + 1, 0, 0));
						}
						// RGB(A) values
						else if (image_data.spectrum() == 3 || image_data.spectrum() == 4)
						{
							for (uint8_t c = 0; c < (uint8_t)image_data.spectrum(); c++)
							{
								background.setChannel(c, image_data(x, y, 0, c));
								foreground.setChannel(c, image_data(x, y + 1, 0, c));
							}
						}
						else
						{
							AR_CORE_ERR("Unknown image file format: ", img_file, "\nImage file has ", image_data.spectrum(), " channels, which is not supported");
						}
						// 9604 = half block
						setTile({ x, y }, Tile(foreground, background, UTF8Char::fromCode(9604)));
					}
				}

				// y height is odd, last row should be handled seperatly
				if (image_data.height() % 2 == 1)
				{
					for (uint32_t x = 0; x < (uint32_t)image_data.width(); x++)
					{

						Colour background;

						if (image_data.spectrum() == 1)
						{
							background = Colour(image_data(x, image_data.height() - 1, 0, 0));
						}
						else if (image_data.spectrum() == 3 || image_data.spectrum() == 4)
						{
							for (uint8_t c = 0; c < (uint8_t)image_data.spectrum(); c++)
								background.setChannel(c, image_data(x, image_data.height() - 1, 0, c));
						}
						else
						{
							AR_CORE_ERR("Unknown image file format: ", img_file, "\nImage file has ", image_data.spectrum(), " channels, which is not supported");
						}

						setTile({ x, size().y - 1 }, Tile(background, Colour(0, 0, 0, 0), UTF8Char::fromCode(9604)));
					}
				}

			}
			else
			{
				resize({ image_data.width(), image_data.height() }, RESIZE::CLEAR);

				for (uint32_t x = 0; x < (uint32_t)image_data.width(); x++)
				{
					for (uint32_t y = 0; y < (uint32_t)image_data.height(); y++)
					{
						Colour colour;
						Tile loaded_tile;

						if (image_data.spectrum() == 1)
						{
							colour = Colour(image_data(x, image_data.height() - 1, 0, 0));
						}
						else if (image_data.spectrum() == 3 || image_data.spectrum() == 4)
						{
							for (uint8_t c = 0; c < (uint8_t)image_data.spectrum(); c++)
								colour.setChannel(c, image_data(x, y, 0, c));
						}
						else
						{
							AR_CORE_ERR("Unknown image file format: ", img_file, "\nImage file has ", image_data.spectrum(), " channels, which is not supported");
						}

						if (load_foreground)
							loaded_tile.colour = colour;
						else
						{
							loaded_tile.background_colour = colour;
							loaded_tile.colour = Colour(0, 0, 0, 0);
						}

						setTile({ x, y }, loaded_tile);
					}
				}
			}

			m_file_dir = img_file;
			m_is_loaded = true;
		}
		catch (CImgIOException e)
		{
			// is probably a unsupported format
			AR_CORE_ERR("Unable to load image file: ", img_file, "\nError message: ", e.what());
		}

		return *this;
	}

	FileTexture& FileTexture::unload()
	{
		AR_ASSERT_MSG(m_is_loaded, "cannot unload already unloaded texture");
		m_is_loaded = false;

		resize({ 0, 0 });

		return *this;
	}

	FileTexture& FileTexture::reload()
	{
		AR_ASSERT_MSG(m_is_loaded, "cannot reload unloaded texture");
		m_is_loaded = false;
		load(m_file_dir);

		return *this;
	}

	Texture2D loadImage(Path image_path, bool load_foreground, bool use_half_tiles)
	{
		CT_MEASURE_N("Image Load");
		AR_ASSERT_VOLATILE(std::filesystem::exists(image_path), "Could not find image file: ", image_path);

		Texture2D image_texture;
		
		try
		{
			CImg<uint8_t> image_data(image_path.string().c_str());

			if (use_half_tiles)
			{
				image_texture.resize({ image_data.width(), (image_data.height() + 1) / 2 - 1 }, RESIZE::CLEAR);

				for (uint32_t x = 0; x < (uint32_t) image_data.width(); x++)
				{
					for (uint32_t y = 0; y < (uint32_t) image_data.height() / 2; y++)
					{
						Colour foreground;
						Colour background;

						/// grayscale values
						if (image_data.spectrum() == 1)
						{
							background = Colour(image_data(x, y, 0, 0));
							foreground = Colour(image_data(x, y + 1, 0, 0));
						}
						// RGB(A) values
						else if (image_data.spectrum() == 3 || image_data.spectrum() == 4)
						{
							for (uint8_t c = 0; c < (uint8_t)image_data.spectrum(); c++)
							{
								background.setChannel(c, image_data(x, y, 0, c));
								foreground.setChannel(c, image_data(x, y + 1, 0, c));
							}
						}
						else
						{
							AR_CORE_ERR("Unknown image file format: ", image_path, "\nImage file has ", image_data.spectrum(), " channels, which is not supported");
							return {};
						}
;
						image_texture.setTile({ x, y }, Tile(foreground, background, UTF8Char::fromCode(9604)));
					}
				}

				// y height is odd, last row should be handled seperatly
				if (image_data.height() % 2 == 1)
				{
					for (uint32_t x = 0; x < (uint32_t)image_data.width(); x++)
					{

						Colour background;

						if (image_data.spectrum() == 1)
						{
							background = Colour(image_data(x, image_data.height() - 1, 0, 0));
						}
						else if (image_data.spectrum() == 3 || image_data.spectrum() == 4)
						{
							for (uint8_t c = 0; c < (uint8_t)image_data.spectrum(); c++)
								background.setChannel(c, image_data(x, image_data.height() - 1, 0, c));
						}
						else
						{
							AR_CORE_ERR("Unknown image file format: ", image_path, "\nImage file has ", image_data.spectrum(), " channels, which is not supported");
							return {};
						}

						image_texture.setTile({ x, image_texture.size().y - 1 }, Tile(background, Colour(0, 0, 0, 0), UTF8Char::fromCode(9604)));
					}
				}

			}
			else
			{
				image_texture.resize({ image_data.width(), image_data.height() }, RESIZE::CLEAR);

				for (uint32_t x = 0; x < (uint32_t)image_data.width(); x++)
				{
					for (uint32_t y = 0; y < (uint32_t)image_data.height(); y++)
					{
						Colour colour;
						Tile loaded_tile;

						if (image_data.spectrum() == 1)
						{
							colour = Colour(image_data(x, image_data.height() - 1, 0, 0));
						}
						else if (image_data.spectrum() == 3 || image_data.spectrum() == 4)
						{
							for (uint8_t c = 0; c < (uint8_t)image_data.spectrum(); c++)
								colour.setChannel(c, image_data(x, y, 0, c));
						}
						else
						{
							AR_CORE_ERR("Unknown image file format: ", image_path, "\nImage file has ", image_data.spectrum(), " channels, which is not supported");
							return {};
						}

						if (load_foreground)
							loaded_tile.colour = colour;
						else
							loaded_tile.background_colour = colour;

						image_texture.setTile({ x, y }, loaded_tile);
					}
				}
			}
		}
		catch (CImgIOException e)
		{
			// is probably a unsupported format
			AR_CORE_ERR("Unable to load image file: ", image_path, "\nError message: ", e.what());
			return {};
		}

		return image_texture;
	}



	// ============ SpriteSheet ============

	Tile SpriteSheet::readTile(const TermVert& coord, Coord, const DeltaTime&, size_t)
	{
		TermVert sprite_coord = coord;

		if (m_tiled_size != TermVert(-1, -1))
		{
			AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Coordinate out of bounds for Texture2D read.\nCoordinate: ", coord,
				"\nTiled size: ", m_tiled_size,
				"\nSprite size: ", m_sprite_size);

			sprite_coord.x %= m_sprite_size.x;
			sprite_coord.y %= m_sprite_size.y;
		}

		// the coordinate must be withing the range of [(0, 0); tile_size], if not, it will be tiled.
		
		sprite_coord += m_sprite_size.cwiseProduct(m_active_sprite) + m_offset + m_padding.cwiseProduct(m_active_sprite);

		return m_texture(sprite_coord);
	}

	void SpriteSheet::setSprite(Size2D sprite_pos)
	{
		AR_ASSERT_MSG(sprite_pos.x < getSpriteCount().x && sprite_pos.y < getSpriteCount().y, "Sprite position must be inside the sprite sheet!\npos: ", sprite_pos, "\nsprite count: ", getSpriteCount());

		m_active_sprite = sprite_pos;
	}

	Texture2D SpriteSheet::getSprite()
	{
		Texture2D sprite(getSpriteSize());

		for (size_t x = 0; x < getSpriteSize().x; x++)
			for (size_t y = 0; y < getSpriteSize().y; y++)
				sprite.setTile({x, y}, readTile({ x, y }));

		return sprite;
	}

	void SpriteSheet::incrSprite(TInt amount)
	{
		TInt new_indx = (m_active_sprite.x + m_active_sprite.y * getSpriteCount().x + amount) % (getSpriteCount().x * getSpriteCount().y);

		m_active_sprite = TermVert(new_indx % getSpriteCount().x, new_indx / getSpriteCount().x);
	}

	TermVert SpriteSheet::size() const
	{
		return m_tiled_size == TermVert(-1, -1) ? getSpriteSize() : m_tiled_size;
	}
}
