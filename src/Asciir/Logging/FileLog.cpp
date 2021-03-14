#include "FileLog.h"
#include <filesystem>

namespace Asciir
{
	FileLog::FileLog(const std::string& log_name, bool save_file)
		: m_log_file(log_name), m_log_start(getTime()), m_name(log_name), m_save_file(save_file)
	{}

	FileLog::~FileLog()
	{
		m_log_file.close();
		if (!m_save_file)
		{
			std::filesystem::remove(m_name);
		}
	}
}
