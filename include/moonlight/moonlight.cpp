/*
	@title
		moonlight.cpp

	@author
		typedef
*/
/// moonlight_information
#include "moonlight.h"

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
		throw std::runtime_error("FSM not in the same directory as Moonlight.");
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