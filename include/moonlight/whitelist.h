#pragma once

/// std::vector
#include <vector>

/// std::string
#include <string>

struct whitelist_information
{
	/// <summary>
	/// Collection of whitelisted scripts.
	/// </summary>
	std::vector< std::string > whitelist;

	/// <summary>
	/// Assign whitelist.
	/// </summary>
	/// <param name="list">The other whitelist.</param>
	whitelist_information(std::vector< std::string > list)
	{
		whitelist = list;
	}

	/// <summary>
	/// Assign whitelist.
	/// </summary>
	/// <param name="list">The other whitelist.</param>
	auto operator=(std::vector< std::string > list) -> void
	{
		whitelist = list;
	}

	/// <summary>
	/// Is script whitelisted?
	/// </summary>
	/// <param name="name">Name of script.</param>
	/// <returns>true if it is; false otherwise.</returns>
	auto is_whitelisted(std::string name) -> bool
	{
		for (auto& i : whitelist)
		{
			if (!i.compare(name)) return true;
		}

		return false;
	}
};
