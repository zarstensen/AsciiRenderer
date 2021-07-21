#include "arpch.h"
#include "FileLog.h"

namespace Asciir
{
	FileLog::FileLog(const std::string& log_name, bool save_file, bool append_logs)
		: m_log_start(getTime()), m_name(log_name), m_save_file(save_file), is_open(true)
	{
		open(log_name, save_file, append_logs);
	}

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

	void FileLog::open(const std::string& log_name, bool save_file, bool append_logs)
	{
#ifdef AR_DEBUG
		if (is_open)
		{
			throw std::runtime_error("Log is already open");
		}
#endif

		is_open = true;

		if (append_logs)
			m_log_file.open(log_name, std::ios::app);
		else
			m_log_file.open(log_name);

		m_name = log_name;
		m_save_file = save_file;
		m_log_file << std::boolalpha;
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

	const std::string& FileLog::getName() const
	{
		return m_name;
	}

	std::string FileLog::getDir() const
	{
		std::filesystem::path dir = m_name;
		return dir.parent_path().string();
	}
}