/*
	@title
		logger.cpp

	@author
		typedef
*/
/// logger_information
#include "logger.h"

/// std::time
#include <time.h>

/// std::fstream
#include <fstream>

/// std::chrono
#include <chrono>

/// std::sort
#include <algorithm>

/// std::format
#include <format>

/// global declaration
logger_information g_logger;

auto logger_information::add(std::string text) -> void
{
	/// Get the current time.
	auto current_time = std::time(NULL);

	log_information temp;
	temp.date = current_time;
	temp.log = text;

	/// Open log file.
	std::fstream f(file_log, std::ios::app);
	if (f.good())
	{
		struct tm time_buffer;
		char buffer[56];

		localtime_s(&time_buffer, &current_time);
		asctime_s< 56 >(buffer, &time_buffer);

		/// asctime_s places newline (\n) at the end of the buffer for literally no reason.
		buffer[strlen(buffer) - 1] = '\0'; 

		auto lfmt = std::format("[{}] {}", buffer, text);
		f << lfmt << std::endl;
		f.close();

		temp.format = lfmt;
		g_logger.logs.push_back(temp);

		/// Sort logs
		std::sort(g_logger.logs.begin(), g_logger.logs.end(), [](const log_information& a, const log_information& b) -> bool
			{
				return a.date > b.date;
			}
		);
	}
}