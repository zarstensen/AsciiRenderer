#pragma once

#include "FileLog.h"

namespace Asciir
{
	static std::filesystem::path CORE_LOG_DIR = AR_CORE_LOG_DIR;
	static std::filesystem::path CLIENT_LOG_DIR = AR_CLIENT_LOG_DIR;
	/// @brief static class managing multiple FileLog classes, as well as providing shortcut logging methods
	class Log
	{
		/// @brief FileLog for the core Asciir library, meaning any logging related to the library itself will be made using this FileLog
		inline static FileLog s_core_log_out;
		/// @brief FileLog for the client application, meaning any logging related to the user implemented application will be made using this FileLog
		inline static FileLog s_client_log_out;
	public:
		/// @brief initialize the logging class
		/// @param save_core specifies if the core log file should be saved when the application exits.
		/// @param save_client specifies if the client log file should be saved when the application exits.
		/// @param append_logs specifies wether to overwrite or append to any existing logs.
		static void init(bool save_core, bool save_client, bool append_logs);

		/// @brief sets the directory (and name) of the core log file
		static void setCoreLogDir(const std::string& dir);
		/// @brief sets the directory (and name) of the client log file
		static void setClientLogDir(const std::string& dir);

		static const FileLog& getCoreLog();
		static const FileLog& getClientLog();

		/// @brief write a core log with the info level and type (0).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void coreInfo(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(0, "CORE", "INFO", line, file, args...);
		}

		/// @brief write a core log with the notify level and type (1).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void coreNotify(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(1, "CORE", "NOTIFY", line, file, args...);
		}

		/// @brief write a core log with the warning level and type (2).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void coreWarning(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(2, "CORE", "WARNING", line, file, args...);
		}

		/// @brief write a core log with the critical level and type (3).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void coreCritical(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(3, "CORE", "CRITICAL", line, file, args...);
		}

		/// @brief write a core log with the errpr level and type (4).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void coreError(size_t line, const char* file, const T& ... args)
		{
			s_core_log_out.Log(4, "CORE", "ERROR", line, file, args...);
		}

		/// @brief write a client log with the info level and type (0).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void clientInfo(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(0, "CLIENT", "INFO", line, file, args...);
		}

		/// @brief write a client log with the notify level and type (1).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void clientNotify(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(1, "CLIENT", "NOTIFY", line, file, args...);
		}

		/// @brief write a client log with the warning level and type (2).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void clientWarning(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(2, "CLIENT", "WARNING", line, file, args...);
		}

		/// @brief write a client log with the critical level and type (3).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void clientCritical(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(3, "CLIENT", "CRITICAL", line, file, args...);
		}

		/// @brief write a client log with the errpr level and type (4).
		/// @see FileLog::log() 
		template<typename ... T>
		inline static void clientError(size_t line, const char* file, const T& ... args)
		{
			s_client_log_out.Log(4, "CLIENT", "ERROR", line, file, args...);
		}
	};
}

// Core macros

/// @brief automaticly sets the line and file arguments @see Log::coreInfo()
#if AR_CORE_VERBOSITY >= 4
#define AR_CORE_INFO(...)	::Asciir::Log::coreInfo(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_INFO(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::coreNotify()
#if AR_CORE_VERBOSITY >= 3
#define AR_CORE_NOTIFY(...) ::Asciir::Log::coreNotify(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_NOTIFY(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::coreWarning()
#if AR_CORE_VERBOSITY >= 2
#define AR_CORE_WARN(...)	::Asciir::Log::coreWarning(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_WARN(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::coreCritical()
#if AR_CORE_VERBOSITY >= 1
#define AR_CORE_CRIT(...)	::Asciir::Log::coreCritical(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_CRIT(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::coreError()
#if AR_CORE_VERBOSITY >= 0
#define AR_CORE_ERR(...)	::Asciir::Log::coreError(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CORE_ERR(...)	{__VA_ARGS__;}
#endif

// Client macros

/// brief automaticly sets the line and file arguments @see Log::clientInfo()
#if AR_CLIENT_VERBOSITY >= 4
#define AR_INFO(...)	::Asciir::Log::clientInfo(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_INFO(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::clientnotify()
#if AR_CLIENT_VERBOSITY >= 3
#define AR_NOTIFY(...) ::Asciir::Log::clientNotify(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_NOTIFY(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::clientWarning()
#if AR_CLIENT_VERBOSITY >= 2
#define AR_WARN(...)	::Asciir::Log::clientWarning(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_WARN(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::clientCritical()
#if AR_CLIENT_VERBOSITY >= 1
#define AR_CRIT(...)	::Asciir::Log::clientCritical(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_CRIT(...)	{__VA_ARGS__;}
#endif

/// @brief automaticly sets the line and file arguments @see Log::clientError()
#if AR_CLIENT_VERBOSITY >= 0
#define AR_ERR(...)	::Asciir::Log::clientError(__LINE__, __FILE__, __VA_ARGS__)
#else
#define AR_ERR(...)	{__VA_ARGS__;}
#endif

#include "ErrLog.ipp"
