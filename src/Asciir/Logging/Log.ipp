#pragma once

namespace Asciir
{
	template<typename ... T>
	inline static void Log::coreInfo(size_t line, const char* file, T ... args)
	{
		s_core_log_out.Log(0, "CORE", "INFO", line, file, args...);
	}
	
	template<typename ... T>
	inline static void Log::coreNotify(size_t line, const char* file, T ... args)
	{
		s_core_log_out.Log(1, "CORE", "NOTIFY", line, file, args...);
	}

	template<typename ... T>
	inline static void Log::coreWarning(size_t line, const char* file, T ... args)
	{
		s_core_log_out.Log(2, "CORE", "WARNING", line, file, args...);
	}

	template<typename ... T>
	inline static void Log::coreCritical(size_t line, const char* file, T ... args)
	{
		s_core_log_out.Log(3, "CORE", "CRITICAL", line, file, args...);
	}
	
	template<typename ... T>
	inline static void Log::coreError(size_t line, const char* file, T ... args)
	{
		s_core_log_out.Log(4, "CORE", "ERROR", line, file, args...);
	}


	template<typename ... T>
	inline static void Log::clientInfo(size_t line, const char* file, T ... args)
	{
		s_client_log_out.Log(0, "CLIENT", "INFO", line, file, args...);
	}

	template<typename ... T>
	inline static void Log::clientNotify(size_t line, const char* file, T ... args)
	{
		s_client_log_out.Log(1, "CLIENT", "NOTIFY", line, file, args...);
	}

	template<typename ... T>
	inline static void Log::clientWarning(size_t line, const char* file, T ... args)
	{
		s_client_log_out.Log(2, "CLIENT", "WARNING", line, file, args...);
	}

	template<typename ... T>
	inline static void Log::clientCritical(size_t line, const char* file, T ... args)
	{
		s_client_log_out.Log(3, "CLIENT", "CRITICAL", line, file, args...);
	}

	template<typename ... T>
	inline static void Log::clientError(size_t line, const char* file, T ... args)
	{
		s_client_log_out.Log(4, "CLIENT", "ERROR", line, file, args...);
	}


}
