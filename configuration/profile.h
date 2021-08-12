/*
	@title
		profile.h

	@author
		typedef
*/

#pragma once

/// std::string
#include <string>

/// moonlight_information
#include "../include/moonlight/moonlight.h"

struct profile_information
{
	/// <summary>
	/// Name of the profile. Accompanied by ".json"
	/// </summary>
	std::string name;

	/// <summary>
	/// The path of Moonlight's binary.
	/// </summary>
	std::string path;

	/// <summary>
	/// The Moonlight data that is unique to the profile.
	/// </summary>
	moonlight_information data;
};