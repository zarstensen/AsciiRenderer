#pragma once

#include "FileLog.h"


namespace Asciir
{
	static std::string CORE_LOG_DIR = AR_CORE_LOG_DIR;
	static std::string CLIENT_LOG_DIR = AR_CLIENT_LOG_DIR;
	
	class Log
	{
		inline static FileLog s_core_log_out;
		inline static FileLog s_client_log_out;
	public:
		static void init(bool save_core, bool save_client, bool append_logs);

		static void setCoreLogDir(const std::string& dir);
		static void setClientLogDir(const std::string& dir);
		
		static const FileLog& getCoreLog();
		static const FileLog& getClientLog();

		template<typename ... T>
		inline static void coreInfo(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(0, "CORE", "INFO", line, file, args...);
		}

		template<typename ... T>
		inline static void coreNotify(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(1, "CORE", "NOTIFY", line, file, args...);
		}

		template<typename ... T>
		inline static void coreWarning(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(2, "CORE", "WARNING", line, file, args...);
		}

		template<typename ... T>
		inline static void coreCritical(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(3, "CORE", "CRITICAL", line, file, args...);
		}

		template<typename ... T>
		inline static void coreError(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(4, "CORE", "ERROR", line, file, args...);
		}

		template<typename ... T>
		inline static void clientInfo(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(0, "CLIENT", "INFO", line, file, args...);
		}

		template<typename ... T>
		inline static void clientNotify(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(1, "CLIENT", "NOTIFY", line, file, args...);
		}

		template<typename ... T>
		inline static void clientWarning(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(2, "CLIENT", "WARNING", line, file, args...);
		}

		template<typename ... T>
		inline static void clientCritical(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(3, "CLIENT", "CRITICAL", line, file, args...);
		}
	
		template<typename ... T>
		inline static void clientError(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(4, "CLIENT", "ERROR", line, file, args...);
		}
	};
}


// Core macros

#ifndef AR_CORE_VERBOSITY
#define AR_CORE_VERBOSITY 4
#endif

#if AR_CORE_VERBOSITY >= 4
#define AR_CORE_INFO(...)	::Asciir::Log::coreInfo(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_INFO(...)	__VA_ARGS__
#endif

#if AR_CORE_VERBOSITY >= 3
#define AR_CORE_NOTIFY(...) ::Asciir::Log::coreNotify(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_NOTIFY(...)	__VA_ARGS__
#endif

#if AR_CORE_VERBOSITY >= 2
#define AR_CORE_WARN(...)	::Asciir::Log::coreWarning(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_WARN(...)	__VA_ARGS__
#endif


#if AR_CORE_VERBOSITY >= 1
#define AR_CORE_CRIT(...)	::Asciir::Log::coreCritical(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_CRIT(...)	__VA_ARGS__
#endif

#if AR_CORE_VERBOSITY >= 0
#define AR_CORE_ERR(...)	::Asciir::Log::coreError(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_ERR(...)	__VA_ARGS__
#endif



// Client macros

#ifndef AR_CLIENT_VERBOSITY
#define AR_CLIENT_VERBOSITY 4
#endif

#if AR_CLIENT_VERBOSITY >= 4
#define AR_INFO(...)	::Asciir::Log::clientInfo(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_INFO(...)	__VA_ARGS__
#endif

#if AR_CLIENT_VERBOSITY >= 3
#define AR_NOTIFY(...) ::Asciir::Log::clientNotify(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_NOTIFY(...)	__VA_ARGS__
#endif

#if AR_CLIENT_VERBOSITY >= 2
#define AR_WARN(...)	::Asciir::Log::clientWarning(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_WARN(...)	__VA_ARGS__
#endif


#if AR_CLIENT_VERBOSITY >= 1
#define AR_CRIT(...)	::Asciir::Log::clientCritical(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CRIT(...)	__VA_ARGS__
#endif

#if AR_CLIENT_VERBOSITY >= 0
#define AR_ERR(...)	::Asciir::Log::clientError(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_ERR(...)	__VA_ARGS__
#endif

#include "ErrLog.ipp"
