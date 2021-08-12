/*
	@title
		logger.h

	@author
		typedef
*/
#pragma once

/// std::vector
#include <vector>

/// log_information
#include "log.h"

struct logger_information
{
	/// <summary>
	/// Log database.
	/// </summary>
	std::vector< log_information > logs;

	/// <summary>
	/// Log file.
	/// </summary>
	const char* file_log = "fsm.log";

	/// <summary>
	/// Add new entry to log database.
	/// </summary>
	/// <param name="text">The log message.</param>
	/// <returns></returns>
	auto add(std::string text) -> void;
};

extern logger_information g_logger;