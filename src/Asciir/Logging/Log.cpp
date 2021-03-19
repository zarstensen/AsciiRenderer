#include "Log.h"

namespace Asciir
{
	void Log::Init()
	{
		s_core_log_out.open(CLIENT_LOG_DIR.string());
		s_client_log_out.open(CORE_LOG_DIR.string());
	}
}
