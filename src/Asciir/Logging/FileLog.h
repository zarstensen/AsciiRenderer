#pragma once

#include "../Core/Core.h"
#include "../Core/Timing.h"
#include <fstream>
#include <chrono>
#include <future>
#include <iomanip>

namespace Asciir
{
	class FileLog
	{
		std::chrono::steady_clock::duration m_log_start;
		std::ofstream m_log_file;
		std::mutex m_log_file_mutex;
		std::string m_name;
		bool m_save_file;

		template<typename ... T>
		void async_log(size_t log_level, const char* log_state, size_t line, const char* file, T ... args);

	public:
		FileLog(const std::string& log_name, bool save_log = true);
		~FileLog();

		template<typename ... T>
		void Log(size_t log_level, const char* log_state, size_t line, const char* file, T ... args);
	};
}

#include "FileLog.ipp"
