/*
	@title
		script.cpp

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

auto moonlight_information::set_scripts_metadata(std::vector< script_information > scripts) -> void
{
	json json_information;
	for (auto& i : scripts)
	{
		json_information["scripts"][i.name]["name"] = i.name;
		json_information["scripts"][i.name]["description"] = i.description;
		json_information["scripts"][i.name]["class"] = i.script_class;
	}

	std::fstream file(directory.string() + file_metadata, std::ios::out | std::ios::binary);
	if (file.good())
	{
		file << json_information.dump();
		file.close();
	}
	else throw std::runtime_error("Could not open meta_data.txt!");
}

auto moonlight_information::get_scripts_metadata(std::vector< script_information >* scripts) -> void
{
	std::fstream file(directory.string() + file_metadata, std::ios::in | std::ios::binary);
	if (file.good())
	{
		try {
			auto json_information = json::parse(file);

			try
			{
				for (auto& i : *scripts)
				{
					i.description = json_information["scripts"][i.name]["description"].get<std::string>();
					i.script_class = json_information["scripts"][i.name]["class"].get<int>();
				}
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
	else throw std::runtime_error("Could not open meta_data.txt!");
}

auto moonlight_information::get_scripts() -> std::vector<script_information>
{
	std::vector<script_information> temporary_script_collection;
	auto whitelist = get_whitelist();

	for (auto& script : std::filesystem::recursive_directory_iterator(directory.string() + "scripts\\"))
	{
		auto path = script.path();
		auto name = script.path().filename().string();

		/// Script folders inside of "scripts"
		if (!path.has_extension()) continue;

		/// Don't add duplicates.
		bool skip = false;
		for (auto& added_script : temporary_script_collection)
		{
			if (!added_script.name.compare(name))
			{
				skip = true;
			}
		}
		if (skip) continue;

		/// Add script.
		script_information temp;
		temp.name = script.path().filename().string();
		temp.path = path.string();
		temp.enabled = true;
		temp.script_class = 0;

		if (path.string().find("\\disabled\\") != std::string::npos) temp.enabled = false;
		temp.whitelisted = whitelist.is_whitelisted(temp.name);

		temporary_script_collection.push_back(temp);
	}

	/// Sort by whitelist.
	std::sort(temporary_script_collection.begin(), temporary_script_collection.end(), [](const script_information& a, const script_information& b) -> bool
		{
			return a.whitelisted && !b.whitelisted;
		}
	);

	return temporary_script_collection;
}