#pragma once

#include <Asciir/Rendering/AsciiAttributes.h>
#include <filesystem>
#include <fstream>

namespace Asciir
{
	class LogViewer
	{
		AsciiAttr m_log_attributes;
		std::filesystem::path m_log_dir;
		std::ifstream m_log_file;
		std::vector<Color> m_colors;
		size_t m_pos = 0;

	public:
		LogViewer(std::filesystem::path log_dir, const std::vector<Color>& colors);
		~LogViewer();

		size_t size();

		void open();
		void close();

		bool hasLogs();
		bool logLineOut(std::ostream& stream);

	};
}

