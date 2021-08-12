/*
	@title
		pure_mode.cpp

	@author
		typedef
*/
/// moonlight_information
#include "moonlight.h"

/// logger
#include "../logger/logger.h"

/// nlohmann:json
#include <nlohmann/json.hpp>

/// json namespace
using json = nlohmann::json;

auto moonlight_information::update_pm() -> void
{
	std::fstream file(directory.string() + "pure_mode.txt", std::ios::in | std::ios::binary);
	if (file.good())
	{
		try {
			auto json_information = json::parse(file);

			try
			{
				pure_mode.allow_non_default_scripts = json_information["Pure Mode"]["allow_non_default_scripts"].get<bool>();
				pure_mode.allow_geometric_changes = json_information["Pure Mode"]["allow_geometric_changes"].get<bool>();
				pure_mode.allow_vac_tools = json_information["Pure Mode"]["allow_vac_tools"].get<bool>();
				pure_mode.always_update = json_information["Pure Mode"]["always_update"].get<bool>();
				pure_mode.hwid_spoofer = json_information["Pure Mode"]["hwid_spoofer"].get<bool>();
				pure_mode.silence = json_information["Pure Mode"]["silence"].get<bool>();
			}
			catch (...)
			{
				g_logger.add("Could not load Pure Mode information.");
			}
		}
		catch (json::parse_error& e)
		{
			g_logger.add(e.what());
		}
		file.close();
	}
	else throw std::runtime_error("Could not open pure_mode.txt!");
}

auto moonlight_information::set_pm() -> void
{
	std::fstream file(directory.string() + "pure_mode.txt", std::ios::out | std::ios::binary);
	if (file.good())
	{
		json json_information;

		json_information["Pure Mode"]["allow_non_default_scripts"] = pure_mode.allow_non_default_scripts;
		json_information["Pure Mode"]["allow_geometric_changes"] = pure_mode.allow_geometric_changes;
		json_information["Pure Mode"]["allow_vac_tools"] = pure_mode.allow_vac_tools;
		json_information["Pure Mode"]["always_update"] = pure_mode.always_update;
		json_information["Pure Mode"]["hwid_spoofer"] = pure_mode.hwid_spoofer;
		json_information["Pure Mode"]["silence"] = pure_mode.silence;

		file << json_information.dump();
		file.close();
	}
	else throw std::runtime_error("Could not open pure_mode.txt!");
}