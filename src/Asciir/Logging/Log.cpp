#include "arpch.h"
#include "Log.h"

namespace Asciir
{
	void Log::init(bool save_core, bool save_client, bool append_logs)
	{
		std::filesystem::create_directories(std::filesystem::absolute(CORE_LOG_DIR).parent_path());
		std::filesystem::create_directories(std::filesystem::absolute(CLIENT_LOG_DIR).parent_path());

		s_core_log_out.open(CORE_LOG_DIR.string(), save_core, append_logs);
		s_client_log_out.open(CLIENT_LOG_DIR.string(), save_client, append_logs);
	}

	void Log::setCoreLogDir(const std::string& dir)
	{
		std::filesystem::path path = std::filesystem::absolute(dir);

		if (path.has_parent_path() && !std::filesystem::exists(path.parent_path())) {
			AR_VERIFY(std::filesystem::create_directories(path.parent_path()));
		}

		s_core_log_out.close();
		s_core_log_out.open(dir);
	}

	void Log::setClientLogDir(const std::string& dir)
	{
		std::filesystem::path path = std::filesystem::absolute(dir);

		if (path.has_parent_path() && !std::filesystem::exists(path.parent_path())) {
			AR_VERIFY(std::filesystem::create_directories(path.parent_path()));
		}

		s_client_log_out.close();
		s_client_log_out.open(dir);
	}

	const FileLog& Log::getCoreLog()
	{
		return s_core_log_out;
	}

	const FileLog& Log::getClientLog()
	{
		return s_client_log_out;
	}
}