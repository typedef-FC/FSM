/*
	@title
		moonlight.h

	@author
		typedef
*/
#pragma once

/// std::filesystem
#include <filesystem>

/// std::fstream
#include <fstream>

/// std::vector
#include <vector>

/// PROCESS_INFORMATION
#include <Windows.h>

/// whitelist_information
#include "whitelist.h"

/// script_information
#include "script.h"

struct moonlight_information
{
	/// <summary>
	/// Moonlight directory.
	/// </summary>
	std::filesystem::path directory;

	/// <summary>
	/// Database of all the scripts.
	/// </summary>
	std::vector< script_information > script_database;

	/// <summary>
	/// File for metadata.
	/// </summary>
	const char* file_metadata = "resources\\fsm\\meta_data.txt";

	/// <summary>
	/// Initialize Moonlight module.
	/// </summary>
	/// <returns></returns>
	auto initialize() -> void;

	/// <summary>
	/// Get whitelist from whitelist.txt.
	/// </summary>
	/// <returns>whitelist_information of whitelisted scripts.</returns>
	auto get_whitelist() -> whitelist_information;

	/// <summary>
	/// Get all scripts in moonlight/scripts folder.
	/// </summary>
	/// <returns>Array/Vector of scripts in script_information.</returns>
	auto get_scripts() -> std::vector< script_information >;

	/// <summary>
	/// Get whitelist.txt as simply a string.
	/// </summary>
	/// <returns>Content of whitelist.txt</returns>
	auto get_whitelist_raw() -> std::string;

	/// <summary>
	/// Sets whitelist.txt content.
	/// </summary>
	/// <param name="buffer">Content to set whitelist.txt.</param>
	/// <returns></returns>
	auto set_whitelist_raw( std::string buffer ) -> void;

	/// <summary>
	/// Set scripts for meta_data.txt.
	/// </summary>
	/// <param name="scripts">Script database to set.</param>
	/// <returns></returns>
	auto set_scripts_metadata(std::vector< script_information > scripts) -> void;

	/// <summary>
	/// Gets scripts from meta_data.txt
	/// </summary>
	/// <param name="scripts">The scripts database to overwrite.</param>
	/// <returns></returns>
	auto get_scripts_metadata(std::vector< script_information > * scripts) -> void;

	/// <summary>
	/// Sets "pure_mode" struct with latest content from pure_mode.txt.
	/// </summary>
	/// <returns></returns>
	auto update_pm() -> void;

	/// <summary>
	/// Sets pure_mode.txt to content of "pure_mode" struct.
	/// </summary>
	/// <returns></returns>
	auto set_pm() -> void;

	/// <summary>
	/// Pure Mode struct.
	/// </summary>
	struct
	{
		bool allow_non_default_scripts = false;
		bool allow_geometric_changes = false;
		bool allow_vac_tools = false;
		bool silence = false;
		bool always_update = true;
		bool hwid_spoofer = true;
	} pure_mode;
};

extern moonlight_information g_moonlight;