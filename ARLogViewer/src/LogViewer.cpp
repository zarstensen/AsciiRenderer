#include "LogViewer.h"
#include <limits>
#undef max
#undef min

namespace Asciir
{
	LogViewer::LogViewer(std::filesystem::path log_dir, const std::vector<Color>& colors)
		: m_log_dir(log_dir), m_colors(colors)
	{}

	LogViewer::~LogViewer()
	{
		close();
	}

	size_t LogViewer::size()
	{
		std::streampos tmp_pos = m_log_file.tellg();
		m_log_file.seekg(0, std::ios::beg);
		m_log_file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize length = m_log_file.gcount();
		m_log_file.clear();
		m_log_file.seekg(tmp_pos, std::ios::beg);

		return length;
	}


	void LogViewer::open()
	{
		m_log_file.open(m_log_dir);
	}

	void LogViewer::close()
	{
		m_log_file.close();
	}


	bool LogViewer::hasLogs()
	{
		return m_pos < size();
	}

	bool LogViewer::logLineOut(std::ostream& stream)
	{
		size_t tmp_pos = m_pos;
		std::string log;

		if (tmp_pos >= size())
		{
			return false;
		}

		m_log_file.ignore(1);

		unsigned int size;
		m_log_file.read((char*)&size, sizeof(unsigned int));

		m_log_file.ignore(1);

		log.resize(size);

		m_log_file.read(log.data(), size);	

		std::string_view log_view = log;

		// get log level
		size_t level = std::stoull((std::string)log_view.substr(1, log_view.find(']')-1));
		level = std::min(level, m_colors.size() - 1);

		m_log_attributes.setForeground(m_colors[level]);
		stream << m_log_attributes << log;
		m_log_attributes.clear();
		stream << m_log_attributes;

		m_pos += 6 + (size_t)size;

		return true;
	}
}
