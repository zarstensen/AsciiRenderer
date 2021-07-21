#pragma once

namespace Asciir
{
	template<typename ...T>
	void FileLog::async_log(size_t log_level, const char* log_source, const char* log_type, size_t line, const char* file, std::tuple<const T& ...>& args)
	{
		std::lock_guard<std::mutex> lock(m_log_file_mutex);
		m_log_file << '[';
		std::stringstream tmp_stream;

		// Format time stamp
		duration time_now = getTime();
		duration time_since_start = time_now - m_log_start;

		// Seconds have a precission of 2 decimals

		long long milliseconds = castMilli(time_since_start);

		long long hours = milliseconds / (1000 * 60 * 60);
		milliseconds -= hours * 1000 * 60 * 60;

		long long minutes = milliseconds / (1000 * 60);
		milliseconds -= minutes * 1000 * 60;

		Real seconds = (Real)milliseconds / 10;
		seconds = round(seconds);
		seconds /= 100;

		tmp_stream << '[' << log_level << ']';

		tmp_stream << std::setfill('0') << '[' <<
			std::setfill('0') << std::setw(2) << hours << "::" <<
			std::setfill('0') << std::setw(2) << minutes << "::" <<
			std::setfill('0') << std::setw(5) << seconds << ']';

		// Log the log level, log stat, line and file it occured on
		tmp_stream << '[' << log_source << ']' << '[' << log_type << ']' << " on line [" << line << "] in source file [" << file << "]:\n";

		write_tuple(tmp_stream, args);

		tmp_stream << "\n\n";

		tmp_stream.seekg(0, std::ios::end);
		unsigned int log_size = (unsigned int)tmp_stream.tellg();
		tmp_stream.seekg(0, std::ios::beg);

		// Flush file so the log can be read instantly from the file
		m_log_file.write((char*)&log_size, sizeof(unsigned int));
		m_log_file << ']' << tmp_stream.rdbuf();
		m_log_file.flush();
	}

	template<typename ...T>
	void FileLog::Log(size_t log_level, const char* log_source, const char* log_type, size_t line, const char* file, const T& ...args)
	{
		std::tuple<const T&...> data(args...);
		auto ret = std::async(std::launch::async, &FileLog::async_log<T...>, this, log_level, log_source, log_type, line, file, std::ref(data));
	}
}