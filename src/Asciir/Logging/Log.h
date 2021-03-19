#pragma once

#include "FileLog.h"
#include <filesystem>


namespace Asciir
{
	static const std::filesystem::path CORE_LOG_DIR = AR_CORE_LOG_DIR;
	static const std::filesystem::path CLIENT_LOG_DIR = AR_CLIENT_LOG_DIR;
	
	class Log
	{
		inline static FileLog s_core_log_out;
		inline static FileLog s_client_log_out;
	public:
		static void Init();

		template<typename ... T>
		inline static void coreInfo(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void coreNotify(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void coreWarning(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void coreCritical(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void coreError(size_t line, const char* file, T ... args);

		template<typename ... T>
		inline static void clientInfo(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void clientNotify(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void clientWarning(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void clientCritical(size_t line, const char* file, T ... args);
		template<typename ... T>
		inline static void clientError(size_t line, const char* file, T ... args);
	};
}

// Core macros
#define AR_CORE_INFO(...)	::Asciir::Log::coreInfo(__LINE__, __FILE__, __VA_ARGS__)
#define AR_CORE_NOTIFY(...) ::Asciir::Log::coreNotify(__LINE__, __FILE__, __VA_ARGS__)
#define AR_CORE_WARN(...)	::Asciir::Log::coreWarning(__LINE__, __FILE__, __VA_ARGS__)
#define AR_CORE_CRIT(...)	::Asciir::Log::coreCritical(__LINE__, __FILE__, __VA_ARGS__)
#define AR_CORE_ERR(...)	::Asciir::Log::coreError(__LINE__, __FILE__, __VA_ARGS__)

// Client macros
#define AR_INFO(...)		::Asciir::Log::coreInfo(__LINE__, __FILE__, __VA_ARGS__)
#define AR_NOTIFY(...)		::Asciir::Log::coreNotify(__LINE__, __FILE__, __VA_ARGS__)
#define AR_WARN(...)		::Asciir::Log::coreWarning(__LINE__, __FILE__, __VA_ARGS__)
#define AR_CRIT(...)		::Asciir::Log::coreCritical(__LINE__, __FILE__, __VA_ARGS__)
#define AR_ERR(...)			::Asciir::Log::coreError(__LINE__, __FILE__, __VA_ARGS__)


#include "Log.ipp"
