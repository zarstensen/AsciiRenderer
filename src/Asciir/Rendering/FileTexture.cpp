#include "arpch.h"
#include "FileTexture.h"

namespace Asciir
{
	void FileTexture::load(const Path& dir)
	{
		Path abs_dir = std::filesystem::absolute(dir);
		AR_ASSERT_MSG(!m_is_loaded, "cannot load file into an already loaded texture");

		if (std::filesystem::exists(abs_dir))
		{
			m_file_dir = abs_dir;
			m_is_loaded = true;
			
			std::ifstream texture_in(abs_dir, std::ios::binary);

			// load the size into memory

			std::cout << sizeof(m_size.x);

			texture_in.read((char*)&m_size.x, sizeof(m_size.x));
			texture_in.read((char*)&m_size.y, sizeof(m_size.y));

			// allocate memory for the texture

			m_data.resize(m_size);

			// load texture into memory
			for (Tile& elem : m_data.reshaped())
			{
				texture_in.read((char*)elem.symbol, 1);
				texture_in.read((char*)elem.symbol + 1, U8CharSize(elem.symbol) - 1);
				texture_in.read((char*)&elem.color, sizeof(elem.color));
				texture_in.read((char*)&elem.background_color, sizeof(elem.background_color));
			}

		}
		else
		{
			AR_CORE_ERR("the file ", dir, " does not exist, unable to load texture");
			AR_ASSERT(false);
		}

	}
	
	void FileTexture::unload()
	{
		AR_ASSERT_MSG(m_is_loaded, "cannot unload already unloaded texture");
		m_is_loaded = false;

		m_data.resize({ 0, 0 });
		m_size = { 0, 0 };
	}

	void FileTexture::reload()
	{
		
	}

}
