/*
	@title
		profile.cpp

	@author
		typedef
*/

//// configuration_information
#include "configuration.h"

/// nlohmann json
#include <nlohmann/json.hpp>

/// json namespace.
using json = nlohmann::json;

auto configuration_information::get_current_profile_data() -> std::unique_ptr< profile_information >&
{
	/// Get current profile.
	for (auto& i : profiles)
	{
		if (!i->name.compare(current_profile))
		{
			/// Found our current profile.
			return i;
		}
	}

	/// This shouldn't be possible.
	throw std::runtime_error("Failed to get current profile data.");
}

auto configuration_information::save_profile() -> void
{
	/// Update our configuration.
	last_profile = current_profile;
	if (current_profile.empty()) current_profile = "default.json";

	/// Read file.
	std::fstream file(g_moonlight.directory.string() + "resources\\fsm\\profiles\\" + current_profile, std::ios::out | std::ios::binary);
	if (!file.good()) throw std::runtime_error("Profile could not be saved.");

	/// Get current profile.
	auto& target_profile = get_current_profile_data();

	/// Create JSON.
	json json_information;
	{
		json_information["path"] = target_profile->path;
		json_information["Pure Mode"]["allow_geometric_changes"] = target_profile->data.pure_mode.allow_geometric_changes;
		json_information["Pure Mode"]["allow_non_default_scripts"] = target_profile->data.pure_mode.allow_non_default_scripts;
		json_information["Pure Mode"]["allow_vac_tools"] = target_profile->data.pure_mode.allow_vac_tools;
		json_information["Pure Mode"]["always_update"] = target_profile->data.pure_mode.always_update;
		json_information["Pure Mode"]["hwid_spoofer"] = target_profile->data.pure_mode.hwid_spoofer;
		json_information["Pure Mode"]["silence"] = target_profile->data.pure_mode.silence;

		for (auto& i : g_moonlight.script_database)
		{
			json_information["scripts"][i.name]["enabled"] = i.enabled;
		}
	}

	/// Write to file and close.
	file << json_information.dump();
	file.close();

	/// Save our FSM settings.
	save_settings();
}

auto configuration_information::load_profile(std::unique_ptr< profile_information >& profile_to_load) -> void
{
	/// Read file.
	std::fstream file(g_moonlight.directory.string() + "resources\\fsm\\profiles\\" + profile_to_load->name, std::ios::in | std::ios::binary);
	if (!file.good()) throw std::runtime_error("Profile could not be loaded.");

	/// Read JSON.
	auto json_information = json::parse(file);
	{
		profile_to_load->data.pure_mode.allow_geometric_changes = json_information["Pure Mode"]["allow_geometric_changes"].get< bool >();
		profile_to_load->data.pure_mode.allow_non_default_scripts = json_information["Pure Mode"]["allow_non_default_scripts"].get< bool >();
		profile_to_load->data.pure_mode.allow_vac_tools = json_information["Pure Mode"]["allow_vac_tools"].get< bool >();
		profile_to_load->data.pure_mode.always_update = json_information["Pure Mode"]["always_update"].get< bool >();
		profile_to_load->data.pure_mode.hwid_spoofer = json_information["Pure Mode"]["hwid_spoofer"].get< bool >();
		profile_to_load->data.pure_mode.silence = json_information["Pure Mode"]["silence"].get< bool >();
		profile_to_load->path = json_information["path"].get< std::string >();

		for (auto& i : json_information["scripts"].items())
		{
			for (auto& script : g_moonlight.script_database)
			{
				if (!script.name.compare(i.key()))
				{
					script.enabled = json_information["scripts"][i.key()]["enabled"].get< bool >();
				}
			}
		}
	}

	/// Close file.
	file.close();

	/// Set current profile.
	current_profile = profile_to_load->name;

	/// Save our FSM settings.
	save_settings();
}
