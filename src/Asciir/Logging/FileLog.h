#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Core/Timing.h"

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
		void async_log(size_t log_level, const char* log_source, const char* log_type, size_t line, const char* file, std::tuple<const T& ...>& args);

	public:
		FileLog();
		FileLog(const std::string& log_name, bool save_log = true, bool append_logs = false);
		~FileLog();

		void open(const std::string& log_name, bool save_log = true, bool append_logs = false);
		void close();

		const std::string& getName() const;
		std::string getDir() const;

		template<typename ... T>
		void Log(size_t log_level, const char* log_source, const char* log_type, size_t line, const char* file, const T& ... args);

	private:
		template<size_t I = 0, typename ... T, std::enable_if_t<I == sizeof...(T), bool> = 0>
		void write_tuple(std::stringstream& sstream, std::tuple<const T&...>&){}

		template<size_t I = 0, typename ... T, std::enable_if_t<I < sizeof...(T), bool> = 0>
			void write_tuple(std::stringstream& sstream, std::tuple<const T&...>& data) { sstream << std::get<I>(data) << ' '; write_tuple<I + 1, T...>(sstream, data); }
	};
}

#include "FileLog.ipp"
