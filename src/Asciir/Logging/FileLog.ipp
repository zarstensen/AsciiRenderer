#pragma once

namespace Asciir
{
	template<typename ...T>
	void FileLog::async_log(size_t log_level, const char* log_state,
		size_t line, const char* file, T ... args)
	{
		std::lock_guard<std::mutex> lock(m_log_file_mutex);

		// Format time stamp
		duration time_now = getTime();
		duration time_since_start = time_now - m_log_start;

		long long milliseconds = convertMilsec(time_since_start);

		// Seconds have a precission of 2 decimals
		long long hours = milliseconds / (1000 * 60 * 60);
		milliseconds -= hours;

		long long minutes = milliseconds / (1000 * 60);
		milliseconds -= minutes;

		float seconds = (float)milliseconds / 10;
		seconds = round(seconds);
		seconds /= 100;

		m_log_file << std::setfill('0') << '[' <<
			std::setw(2) << hours << "::" <<
			std::setw(2) << minutes << "::" <<
			std::setw(4) << seconds << ']';

		// Log the log level, log stat, line and file it occured on
		m_log_file << " [" << log_level << "] " << log_state << " on line [" << line << "] in source file \"" << file << "\":\n";


		(m_log_file << ... << args) << '\n';

		// Flush file so the log can be read instantly from the file
		m_log_file.flush();
	}

	template<typename ...T>
	void FileLog::Log(size_t log_level, const char* log_state, size_t line, const char* file, T ...args)
	{

		auto ret = std::async(std::launch::async, &FileLog::async_log<T...>, this, log_level, log_state, line, file, args...);
	}
}
