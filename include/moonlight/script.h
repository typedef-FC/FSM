/*
	@title
		script.h

	@author
		typedef
*/
#pragma once

/// std::string
#include <string>

enum SCRIPT_CLASS : int
{
	CLASS_NONE,
	CLASS_A,
	CLASS_B,
	CLASS_C,
	CLASS_D,
	CLASS_F
};

struct script_information
{
	/// <summary>
	/// Name of the script.
	/// </summary>
	std::string name;

	/// <summary>
	/// Location of the script.
	/// </summary>
	std::string path;

	/// <summary>
	/// Description of the script.
	/// </summary>
	std::string description;

	/// <summary>
	/// Class of the script.
	/// </summary>
	int script_class;

	/// <summary>
	/// Is checked or not.
	/// </summary>
	bool enabled;

	/// <summary>
	/// Is whitelisted or not.
	/// </summary>
	bool whitelisted;
};