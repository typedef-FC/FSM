/*
	@title
		options.h

	@author
		typedef
*/
#pragma once

/// std::map
#include <map>

/// std::wstring
#include <string>

std::map< const char*, std::wstring > m_tools =
{
	{"Steam Account Switcher", L"SteamAccountSwitcher.exe"},
	{"Steam Idling Tool", L"idle.py"},
};

std::map< const char*, const wchar_t * > m_guides =
{
	{"Installation & Usage", L"https://fantasy.cat/forums/index.php?threads/software-installation-usage-guide.3242/#post-30209"},
	{"Picking the Right Scripts", L"https://fantasy.cat/forums/index.php?threads/picking-the-right-scripts-security-closet-cheating-organic-skill-improvement.4716/"},
	{"Humanizer Behavior Guide", L"https://fantasy.cat/forums/index.php?threads/moonlight-humanizer-behavior-guide-maximize-potential-proper-usage-how-it-works.3869/"},
	{"Checking Safety in Lua Scripts", L"https://fantasy.cat/forums/index.php?threads/checking-safety-in-lua-scripts.4400/"},
	{"How to Win Competitive Games", L"https://fantasy.cat/forums/index.php?threads/how-to-win-in-competitive-games-a-comprehensive-guide.4739/"},
	{"ALL Guides", L"https://fantasy.cat/forums/index.php?forums/general.8/&prefix_id=46"},
};