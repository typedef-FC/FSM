/*
	@title
		whitelist.cpp

	@author
		typedef
*/
/// moonlight_information
#include "moonlight.h"

auto moonlight_information::get_whitelist_raw() -> std::string
{
	/// Read file.
	std::fstream file(directory.string() + "whitelist.txt", std::ios::in | std::ios::binary);

	/// To string.
	if (file.good()) return std::string(((std::istreambuf_iterator<char>(file))), std::istreambuf_iterator<char>());
	else throw std::runtime_error("Could not read whitelist.txt!");

	/// Blank in the case whitelist.txt could not be read.
	return std::string();
}

auto moonlight_information::set_whitelist_raw(std::string buffer) -> void
{
	/// Read file.
	std::fstream file(directory.string() + "whitelist.txt", std::ios::out | std::ios::binary);
	if (file.good())
	{
		file << buffer;
		file.close();
	}
}

auto moonlight_information::get_whitelist() -> whitelist_information
{
	std::vector< std::string > return_whitelist;

	/// Read file.
	std::fstream file(directory.string() + "whitelist.txt", std::ios::in | std::ios::binary);
	if (file.good())
	{
		std::string script_name;

		/// Read line by line.
		while (std::getline(file, script_name))
		{
			/// Remove \r
			script_name = script_name.substr(0, script_name.size() - 1);

			/// Add .lua extension.
			script_name += ".lua";

			/// Add to database.
			return_whitelist.push_back(script_name);
		}

		file.close();
	}
	else throw std::runtime_error("Could not read whitelist.txt!");

	/// Return our whitelist.
	return return_whitelist;
}