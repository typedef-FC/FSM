/*
	@title
		moonlight.cpp

	@author
		typedef
*/
/// moonlight_information
#include "moonlight.h"

/// configuration_data
#include "../../configuration/configuration.h"

/// logger
#include "../logger/logger.h"

/// nlohmann:json
#include <nlohmann/json.hpp>

/// global declaration
moonlight_information g_moonlight;

/// json namespace
using json = nlohmann::json;

auto moonlight_information::initialize() -> void
{
	/// Get our Moonlight directory.
	directory = std::filesystem::current_path().concat(L"\\moonlight\\");
	if (!std::filesystem::exists(directory))
	{
		/// Okay, we're not in the same directory as fantasy.moonlight.exe it seems. Let's try our parent path?
		directory = std::filesystem::current_path().parent_path().concat(L"\\moonlight\\");
		if (!std::filesystem::exists(directory))
		{
			throw std::runtime_error("FSM is unable to find Moonlight.");
		}
	}

	/// Check if the required files exist.
	std::vector< std::wstring > files_to_check =
	{
		L"whitelist.txt",
		L"pure_mode.txt"
	};

	for (const auto& i : files_to_check)
	{
		auto temporary_directory = directory;
		if (!std::filesystem::exists(temporary_directory.concat(i)))
		{
			throw std::runtime_error("Moonlight is not properly installed.");
		}
	}
}

auto moonlight_information::start() -> void
{
	/// Get current profile.
	auto& current_profile = g_configuration.get_current_profile_data();

	/// Save our profile settings.
	g_configuration.save_profile();

	/// The user didn't set the Moonlight binary path yet. Nothing to execute.
	if (current_profile->path.empty())
	{
		g_logger.add("Missing Moonlight binary path. Please go to Profiles -> Moonlight Binary.");
		return;
	}

	/// Update pure_mode.txt with latest profile information. This could be done better***
	g_moonlight.pure_mode = current_profile->data.pure_mode;
	g_moonlight.set_pm();

	/*
		Here we are organizing the scripts.

		1) Create a new directory where we place all the enabled scripts.
		2) Move the enabled scripts to the root folder.
		3) Delete the directory.
	*/
	auto created_directory = g_moonlight.directory.string() + "fsm";
	auto scripts_directory = g_moonlight.directory.string() + "\\scripts\\";

	/// #1 - Remove the directory just in case it exists already.
	std::filesystem::remove_all(created_directory.c_str());
	if (!std::filesystem::create_directory(created_directory.c_str()))
	{
		g_logger.add("Failed to create directory to start Moonlight.");
		return;
	}

	/// Move the enabled scripts to our new directory.
	for (auto& i : g_moonlight.script_database)
	{
		/// Only enabled scripts.
		if (!i.enabled) continue;

		/// Core scripts and library scripts are always on.
		if (i.path.find("\\core\\") != std::string::npos || i.path.find("\\lib\\") != std::string::npos) continue;

		/// Copy now.
		std::filesystem::copy(i.path, created_directory + "\\" + i.name);
	}

	/// #2 - Move the enabled scripts.
	for (auto& i : std::filesystem::directory_iterator(scripts_directory))
	{
		/// Ignore files without extensions.
		if (!i.path().has_extension()) continue;

		/// Copy the files to disabled or else we can permanently lose scripts.
		std::filesystem::copy(i.path().string(), scripts_directory + "disabled\\", std::filesystem::copy_options::overwrite_existing);

		/// Delete the files already inside of moonlight/scripts.
		std::filesystem::remove(i.path().string());
	}

	/// Move our temporarily stored scripts to our root directory.
	for (auto& i : std::filesystem::directory_iterator(created_directory))
	{
		std::filesystem::copy(i.path(), scripts_directory, std::filesystem::copy_options::overwrite_existing);
	}

	/// #3 - Delete directory.
	std::filesystem::remove_all(created_directory.c_str());

	/// Launch Moonlight based on the executable path provided.
	STARTUPINFOA startup_info =
	{
		sizeof(startup_info)
	};
	PROCESS_INFORMATION process_info;

	/// CreateProcess, but launch with "CREATE_NEW_CONSOLE" otherwise it will hijack the current process console if applicable.
	if (!CreateProcessA(current_profile->path.c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startup_info, &process_info))
	{
		g_logger.add("Process could not start.");
		return;
	}

	/*
		Log and pop up that the process started.
		TODO: Work like Mod Organizer and disable the GUI instead of a verbal request.
	*/
	auto pfmt = std::format("Process started (Process ID: {}). Please don't make further changes in FSM until Moonlight is closed.", process_info.dwProcessId);
	g_logger.add(pfmt);
	MessageBoxA(NULL, pfmt.c_str(), "FSM", MB_OK);
}
