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
		bool is_open;

		template<typename ... T>
		void async_log(size_t log_level, const char* log_cource, const char* log_type, size_t line, const char* file, T ... args);

	public:
		FileLog();
		FileLog(const std::string& log_name, bool save_log = true);
		~FileLog();

		void open(const std::string& log_name, bool save_log = true);
		void close();

		template<typename ... T>
		void Log(size_t log_level, const char* log_source, const char* log_type, size_t line, const char* file, T ... args);
	};
}

#include "FileLog.ipp"
