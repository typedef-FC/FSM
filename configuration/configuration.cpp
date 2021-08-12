/*
	@title
		configuration.cpp

	@author
		typedef
*/

/// configuration_information
#include "configuration.h"

/// g_logger
#include "../include/logger/logger.h"

/// nlohmann json
#include <nlohmann/json.hpp>

/// json namespace
using json = nlohmann::json;

/// global declaration
configuration_information g_configuration;

auto configuration_information::initialize() -> void
{
	/// Read file.
	std::fstream file(g_moonlight.directory.string() + file_configuration, std::ios::in | std::ios::binary);

	/// Configuration file doesn't exist. That's okay.
	if (!file.good())
	{
		std::fstream file_out(g_moonlight.directory.string() + file_configuration, std::ios::out | std::ios::binary);

		if (file_out.good())
		{
			/// Create default FSM settings.
			json json_information;
			json_information["configuration"]["last_profile"] = "default.json";

			/// Write to file and close.
			file_out << json_information.dump();
			file_out.close();

			/// Set last profile.
			last_profile = "default.json";
		}
		else throw std::runtime_error("Could not process configuration file.");
	}
	else
	{
		try
		{
			/// Process our configuration.
			json json_information = json::parse(file);

			try
			{
				last_profile = json_information["configuration"]["last_profile"].get< std::string >();
			}
			catch (...)
			{
				throw std::runtime_error("Failed to load configuration file.");
			}
		}
		catch (json::parse_error& e)
		{
			throw std::runtime_error(e.what());
		}

		file.close();
	}

	/// Load all profiles.
	for (auto& profile : std::filesystem::directory_iterator(g_moonlight.directory.string() + "resources\\fsm\\profiles\\"))
	{
		profile_information temp;
		temp.name = profile.path().filename().string();
		
		g_logger.add("Loading profile " + temp.name);

		/// Just add without the moonlight_information or path. Save launch time for multiple profiles. 
		profiles.push_back(std::make_unique< profile_information >( temp ));
	}

	/// No profiles in the profile folder.
	if (profiles.empty())
	{
		g_logger.add("No profiles found.");

		/// Create default profile.
		current_profile = "default.json";

		profile_information temp;
		temp.name = "default.json";
		profiles.push_back(std::make_unique< profile_information >(temp));

		/// Create default.json.
		save_profile( );
		return;
	}

	/// We have profiles. But does our last_profile exist?
	bool found_last_used_profile = false;
	for (auto& i : profiles)
	{
		if (!i->name.compare(last_profile))
		{
			found_last_used_profile = true;
			break;
		}
	}

	/// Couldn't find our last profile. 
	if (!found_last_used_profile)
	{
		g_logger.add("The last used profile was not found.");

		/// Used the first profile in our directory.
		last_profile = profiles[0]->name;	
	}

	/// Set our current profile to "last_profile".
	current_profile = last_profile;
}

auto configuration_information::save_settings() -> void
{
	/// Read file.
	std::fstream file(g_moonlight.directory.string() + file_configuration, std::ios::in | std::ios::binary);

	/// Configuration file missing somehow.
	if (!file.good()) throw std::runtime_error("Configuration file could not be opened.");

	/// Set our settings.
	json json_information;
	json_information["configuration"]["last_profile"] = current_profile;

	/// Write to file.
	file << json_information.dump();

	/// Close file.
	file.close();
}