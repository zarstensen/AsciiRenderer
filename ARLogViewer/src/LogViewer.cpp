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

		return (size_t)length;
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
		size_t level_indx = log_view.find(']');
		size_t level = (size_t)std::stoull((std::string)log_view.substr(1, level_indx - 1));
		level = std::min(level, m_colors.size() - 1);

		size_t time_indx = log_view.find(']', level_indx + 1);
		size_t type_indx = log_view.find(']', time_indx + 1);
		size_t line_indx = log_view.find(']', type_indx + 1);
		size_t file_indx = log_view.find(']', line_indx + 1);

		std::cout << time_indx << ',' << type_indx << ',' << line_indx << ',' << file_indx << '\n';

		m_log_attributes.setForeground(m_colors[level]);
		stream << m_log_attributes << log.substr(level_indx + 2, time_indx - level_indx);
		m_log_attributes.setAttribute(BOLD, true);
		stream << m_log_attributes << log.substr(time_indx + 3, type_indx - time_indx - 3);
		m_log_attributes.setAttribute(BOLD, false);
		stream << m_log_attributes << log.substr(type_indx + 1);
		m_log_attributes.clear();
		stream << m_log_attributes;

		m_pos += 6 + (size_t)size;

		return true;
	}
}