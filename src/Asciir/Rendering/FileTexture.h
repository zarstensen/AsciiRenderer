#pragma once

#include "Shader.h"

namespace Asciir
{

	typedef std::filesystem::path Path;

	// stores data from a .cart (compact asciir Texture) file
	class FileTexture: public Shader
	{
	public:
		FileTexture() = default;
		FileTexture(const Path& file_dir) { load(file_dir); };

		~FileTexture() final override { if (loaded()) unload(); }

		Size2D size() const final override { return m_size; }
		Tile readTile(const Size2D& coord, const DeltaTime& time_since_start, const size_t& frames_since_start) const final override { return m_data(coord); }

		void load(const Path& dir);
		void unload();
		
		void reload();

		bool loaded() const { return m_is_loaded; }

		Path dir() const { return m_file_dir; };

	protected:
		Path m_file_dir;

		Size2D m_size;

		arMatrix<Tile> m_data;

		bool m_is_loaded = false;
	};
}

