#include "FileLog.h"
#include <filesystem>

namespace Asciir
{
	FileLog::FileLog(const std::string& log_name, bool save_file)
		: m_log_file(log_name), m_log_start(getTime()), m_name(log_name), m_save_file(save_file), is_open(true)
	{}

	FileLog::FileLog()
		: m_log_start(getTime()), m_save_file(false), is_open(false)
	{}

	FileLog::~FileLog()
	{
		if (is_open)
		{
			close();
		}
	}

	void FileLog::open(const std::string& log_name, bool save_file)
	{
		#ifdef AR_DEBUG
		if (is_open)
		{
			throw std::runtime_error("Log is already open");
		}
		#endif

		is_open = true;

		m_log_file.open(log_name);
		m_name = log_name;
		m_save_file = save_file;
	}

	void FileLog::close()
	{

		#ifdef AR_DEBUG
		if (!is_open)
		{
			throw std::runtime_error("Log is already closed");
		}
		#endif

		is_open = false;

		m_log_file.close();
		if (!m_save_file)
		{
			std::filesystem::remove(m_name);
		}
	}

}