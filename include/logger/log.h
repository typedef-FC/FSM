/*
	@title
		log.h

	@author
		typedef
*/
#pragma once

/// std::string
#include <string>

struct log_information
{
	/// <summary>
	/// Timestamp of the log.
	/// </summary>
	long long date;

	/// <summary>
	/// Log message. Unused.
	/// </summary>
	std::string log;

	/// <summary>
	/// Log message formatted.
	/// </summary>
	std::string format;
};