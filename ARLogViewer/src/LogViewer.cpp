#include <pch/arpch.h>
#include "LogViewer.h"
#include <limits>

namespace Asciir
{
	LogViewer::LogViewer(std::filesystem::path log_dir, const std::vector<Color>& colors)
		: m_log_dir(log_dir), m_colors(colors)
	{

		#ifdef AR_WIN
		m_log_attributes = std::make_unique<WinARAttr>();
		#elif defined(AR_UNIX)
		m_log_attributes = std::make_unique<UnixARAttr>();
		#endif

		m_log_attributes->setTitle("LogViewer");
	}

	LogViewer::~LogViewer()
	{
		close();
	}

	size_t LogViewer::pos()
	{
		return m_pos;
	}

	size_t LogViewer::size()
	{
		size_t length = (size_t) std::filesystem::file_size(m_log_dir);
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
		m_log_attributes->clear();
		size_t tmp_pos = m_pos;
		std::string log;

		if (tmp_pos + 1 + sizeof(unsigned int) >= size())
		{
			return false;
		}

		std::streampos fail_pos = m_log_file.tellg();
		m_log_file.ignore(1);

		unsigned int l_size;
		m_log_file.read((char*)&l_size, sizeof(unsigned int));

		if (l_size > size())
		{
			m_log_file.clear();
			m_log_file.seekg(fail_pos, std::ios::beg);
			return false;
		}

		m_log_file.ignore(1);

		log.resize(l_size);

		m_log_file.read(log.data(), l_size);

		std::string_view log_view = log;

		// get log level
		size_t level_indx = log_view.find(']');
		size_t level = (size_t)std::stoull((std::string)log_view.substr(1, level_indx - 1));
		level = (std::min)(level, m_colors.size() - 1);
		std::cout << level;

		size_t time_indx = log_view.find(']', level_indx + 1);
		size_t source_indx = log_view.find(']', time_indx + 1);
		size_t type_indx = log_view.find(']', source_indx + 1);
		size_t line_indx = log_view.find(']', type_indx + 1);
		size_t file_indx = log_view.find(']', line_indx + 1);

		m_log_attributes->setForeground(m_colors[level]);
		stream << m_log_attributes << log.substr(level_indx + 1, time_indx - level_indx);

		stream << log.substr(time_indx + 1, source_indx - time_indx) << ": ";
		stream << log.substr(file_indx + 3);

		m_log_attributes->clear();
		stream << m_log_attributes;

		m_pos += 6 + (size_t)l_size;

		return true;
	}

	void LogViewer::reset(std::ostream& stream)
	{
		stream.clear();

		m_pos = 0;
		m_log_file.seekg(std::ios::beg);
		
	}
}