/*
	@title
		configuration.h

	@author
		typedef
*/

#pragma once

/// profile_information
#include "profile.h"

/// std::vector
#include <vector>

/// std::unique_ptr
#include <memory>

struct configuration_information
{
	/// <summary>
	/// Default configuration file.
	/// </summary>
	const char * file_configuration = "resources\\fsm\\configuration.json";

	/// <summary>
	/// Storage of all profiles.
	/// </summary>
	std::vector< std::unique_ptr< profile_information > > profiles;

	/// <summary>
	/// last_profile in configuration file. Usused globally.
	/// </summary>
	std::string last_profile;

	/// <summary>
	/// Currently loaded profile name.
	/// </summary>
	std::string current_profile;

	/// <summary>
	///	Initalizes the configuration module. Will create configuration file if it doesn't exist and will handle profiles.
	/// </summary>
	/// <returns></returns>
	auto initialize() -> void;

	/// <summary>
	/// Saves the FSM configuration.
	/// </summary>
	/// <returns></returns>
	auto save_settings() -> void;

	/// <summary>
	/// Saves the CURRENT profile.
	/// </summary>
	/// <param name="scripts">The script database to save to the profile.</param>
	/// <returns></returns>
	auto save_profile() -> void;

	/// <summary>
	/// Loads a profile.
	/// </summary>
	/// <param name="profile_to_load">The profile information.</param>
	/// <returns></returns>
	auto load_profile(std::unique_ptr< profile_information > & profile_to_load) -> void;

	/// <summary>
	/// Gets the currently loaded profile data based on current_profile.
	/// </summary>
	/// <returns>The profile data.</returns>
	auto get_current_profile_data() -> std::unique_ptr< profile_information > &;
};

extern configuration_information g_configuration;