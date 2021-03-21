#include "arpch.h"
#include "Log.h"

namespace Asciir
{
	void Log::Init(bool save_core, bool save_client, bool append_logs)
	{
		s_core_log_out.open(CORE_LOG_DIR.string(), save_core, append_logs);
		s_client_log_out.open(CLIENT_LOG_DIR.string(), save_client, append_logs);
	}
}
