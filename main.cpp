/*
	@title
		main.h

	@author
		typedef

	@notes
		Includes the start, GUI render and actions.
*/

/// moonlight_information
#include "include/moonlight/moonlight.h"

/// gui_information + imgui + menu options
#include "gui/gui.h"
#include "gui/options.h"

/// logger_information
#include "include/logger/logger.h"

/// configuration_information
#include "configuration/configuration.h"

/// std::format
#include <format>

int _stdcall WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	try 
	{
		/// Start core features.
		{
			g_logger.add("FSM launched.");
			g_moonlight.initialize();

			g_logger.add("Getting scripts.");
			g_moonlight.script_database = g_moonlight.get_scripts();

			g_logger.add("Updating metadata.");
			g_moonlight.get_scripts_metadata(&g_moonlight.script_database);

			g_logger.add("Getting configuration and profiles.");
			g_configuration.initialize();

			g_logger.add("Loading last known configuration...");
			g_configuration.load_profile(g_configuration.get_current_profile_data());

			g_logger.add("Starting GUI.");
			g_gui.initialize();
		}

		/// For metadata
		static const char* class_options[] =
		{
			"Unassigned",
			"Class A",
			"Class B",
			"Class C",
			"Class D",
			"Class F"
		};

		/// Ready to roll.
		g_logger.add("FSM initialized.");

		/// Render GUI
		while (g_gui.start_frame())
		{
			// ImGui window match GUI window size.
			ImGui::SetNextWindowSize( ImVec2( 1280, 800 ) );
			ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );

			/// Get current profile information.
			auto& current_profile = g_configuration.get_current_profile_data();

			/// Create Window
			if (ImGui::Begin("##main_window", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize ))
			{
				/// Create Menu Bar
				{
					if (ImGui::BeginMenuBar())
					{
						if (ImGui::BeginMenu("File"))
						{
							if (ImGui::BeginMenu("Profiles"))
							{
								/// Loop through all profiles.
								for (auto& i : g_configuration.profiles)
								{
									/// Create radio buttons. Only set active to one that matches the current profile.
									if (ImGui::RadioButton(i->name.c_str(), !i->name.compare(g_configuration.current_profile)))
									{
										/// Load profile if button selected.
										g_configuration.load_profile(i);
									}
								}
								ImGui::EndMenu();
							}

							if (ImGui::MenuItem("Exit"))
							{
								break;
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Tools"))
						{
							/// Loop through all our tools and create a way to open them.
							for (auto& i : m_tools)
							{
								if (ImGui::MenuItem(i.first))
								{
									ShellExecute(NULL, L"open", std::wstring(g_moonlight.directory.wstring() + L"\\resources\\fsm\\tools\\" + i.second).c_str(), NULL, NULL, SW_SHOWNORMAL);
								}
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Guides"))
						{
							/// Loop through all our guides and create a hyperlink that we can open.
							for (auto& i : m_guides)
							{
								if (ImGui::MenuItem(i.first))
								{
									ShellExecute(NULL, L"open", i.second, NULL, NULL, SW_SHOWNORMAL);
								}
							}
							ImGui::EndMenu();
						}

						ImGui::EndMenuBar();
					}
				}

				/// Create list of scripts.
				{
					ImGui::PushItemWidth(300);
					if (ImGui::BeginTable("All Scripts", 4, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_ScrollY, ImVec2(790, 600)))
					{
						/// Set to start at first column.
						ImGui::TableNextColumn();
						{
							if (ImGui::Selectable("Script"))
							{
								/// Sort by name.
								std::sort(g_moonlight.script_database.begin(), g_moonlight.script_database.end(), [](const script_information& a, const script_information& b) -> bool
									{
										return a.name < b.name;
									}
								);
							}
							ImGui::TableNextColumn();

							if (ImGui::Selectable("Description"))
							{
								/// Sort by description.
								std::sort(g_moonlight.script_database.begin(), g_moonlight.script_database.end(), [](const script_information& a, const script_information& b) -> bool
									{
										return a.description < b.description;
									}
								);
							}
							ImGui::TableNextColumn();

							if (ImGui::Selectable("Class"))
							{
								/// Sort by script class.
								std::sort(g_moonlight.script_database.begin(), g_moonlight.script_database.end(), [](const script_information& a, const script_information& b) -> bool
									{
										return a.script_class < b.script_class;
									}
								);
							}
							ImGui::TableNextColumn();

							if (ImGui::Selectable("Whitelisted"))
							{
								/// Sort by whitelist.
								std::sort(g_moonlight.script_database.begin(), g_moonlight.script_database.end(), [](const script_information& a, const script_information& b) -> bool
									{
										return a.whitelisted && !b.whitelisted;
									}
								);
							}
							ImGui::TableNextColumn();
						}

						/*
							Loop through all the scripts we have stored. 
							Don't allow scripts in "core" and "lib" to be toggled. They should always be running.
 						*/
						for (auto& i : g_moonlight.script_database)
						{
							/// Start row.
							ImGui::TableNextRow();

							/// Disallow "core" and "lib" scripts from being toggled. 
							auto disallow_script_toggle = i.path.find("\\core\\") != std::string::npos || i.path.find("\\lib\\") != std::string::npos;

							/// Scripts -> Script Name
							{
								ImGui::TableNextColumn();
								if (disallow_script_toggle)
								{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}

								ImGui::Checkbox(i.name.c_str(), &i.enabled);

								if (disallow_script_toggle)
								{
									ImGui::PopStyleVar();
									ImGui::PopItemFlag();
								}
							}

							/// Scripts -> Description
							{
								ImGui::TableNextColumn();
								if (!i.description.length())
								{
									ImGui::Text("None");
								}
								else
								{
									ImGui::Text(i.description.c_str());
								}
							}

							/// Scripts -> Class
							{
								ImGui::TableNextColumn();

								switch (i.script_class)
								{
									case CLASS_NONE:
									{
										ImGui::TextColored(ImVec4(1, 1, 1, 1), class_options[i.script_class]);
										break;
									}
									case CLASS_A:
									{
										ImGui::TextColored(ImVec4(0, 1, 0, 1), class_options[i.script_class]);
										break;
									}
									case CLASS_B:
									{
										ImGui::TextColored(ImVec4(0, 0.5f, 0, 1), class_options[i.script_class]);
										break;
									}
									case CLASS_C:
									{
										ImGui::TextColored(ImVec4(0, 1, 1, 1), class_options[i.script_class]);
										break;
									}
									case CLASS_D:
									{
										ImGui::TextColored(ImVec4(0.5f, 0, 0, 1), class_options[i.script_class]);
										break;
									}
									case CLASS_F:
									{
										ImGui::TextColored(ImVec4(1, 0, 0, 1), class_options[i.script_class]);
										break;
									}
								}
							}

							/// Scripts -> Whitelist Status
							{
								ImGui::TableNextColumn();
								i.whitelisted ? ImGui::Text("Yes") : ImGui::Text("No");
							}
						}

						ImGui::EndTable();
					}

					ImGui::PopItemWidth();
				}
				
				/// Give summary information about your profile.
				{
					/// Set position in GUI.
					ImGui::SetCursorPos(ImVec2(800, 30));

					/// Count how many scripts we have enabled right now.
					int loaded_scripts = 0;
					for (auto& i : g_moonlight.script_database)
					{
						if (i.enabled)
						{
							loaded_scripts++;
						}
					}

					/// Display.
					ImGui::Text("Profile: %s\nScripts Loaded: %i\nIgnore Whitelist: %s\nGeometric Changes: %s\nSilence: %s\nVAC Tools: %s\nHWID Spoofer: %s",
						current_profile->name.c_str(),
						loaded_scripts,
						current_profile->data.pure_mode.allow_non_default_scripts ? "On" : "Off",
						current_profile->data.pure_mode.allow_geometric_changes ? "On" : "Off",
						current_profile->data.pure_mode.silence ? "On" : "Off",
						current_profile->data.pure_mode.allow_vac_tools ? "On" : "Off",
						current_profile->data.pure_mode.hwid_spoofer ? "On" : "Off"
					);
				}

				/// The dropdown menu where you can select FSM's actions.
				{
					/// Set position in GUI.
					ImGui::SetCursorPos(ImVec2(1000, 30));

					/// List of actions.
					static const char* executable_options[] =
					{
						"Start Moonlight (Standalone)",
						"Start Moonlight (CS:GO)",
						"Start Moonlight (TF2)",
						"Read Patch Notes",
						"Open General Logs",
						"Open Warning Logs",
						"Open Error Logs"
					};
					static int current_selected_action = 0;

					ImGui::PushItemWidth(250);
					ImGui::Combo("##executable_action", &current_selected_action, executable_options, IM_ARRAYSIZE(executable_options), IM_ARRAYSIZE(executable_options));
					ImGui::PopItemWidth();

					/// Position button.
					ImGui::SetCursorPos(ImVec2(1000, 60));

					/// Execute now.
					if (ImGui::Button("Execute", ImVec2(250, 35)))
					{
						switch (current_selected_action)
						{
							case 0: /// Start Moonlight
							{
								g_moonlight.start();
								break;
							}

							case 1: /// Start Moonlight (CS:GO)
							{
								ShellExecute(NULL, L"open", L"steam://rungameid/730/", NULL, NULL, SW_SHOWNORMAL);
								g_moonlight.start();
								break;
							}

							case 2: /// Start Moonlight (TF2)
							{
								ShellExecute(NULL, L"open", L"steam://rungameid/440/", NULL, NULL, SW_SHOWNORMAL);
								g_moonlight.start();
								break;
							}

							case 3: /// Read Patch Notes
							{
								ShellExecute(NULL, L"open", L"https://fantasy.cat/update/index.php?c=Moonlight", NULL, NULL, SW_SHOWNORMAL);
								break;
							}

							case 4: /// Open General Logs
							{
								ShellExecute(NULL, L"open", std::wstring( g_moonlight.directory.wstring() + L"\\logs\\log.txt" ).c_str(), NULL, NULL, SW_SHOWNORMAL);
								break;
							}

							case 5: /// Open Warning Logs
							{
								ShellExecute(NULL, L"open", std::wstring(g_moonlight.directory.wstring() + L"\\logs\\warning.txt").c_str(), NULL, NULL, SW_SHOWNORMAL);
								break;
							}

							case 6: /// Open Error Logs
							{
								ShellExecute(NULL, L"open", std::wstring(g_moonlight.directory.wstring() + L"\\logs\\error.txt").c_str(), NULL, NULL, SW_SHOWNORMAL);
								break;
							}
						}
					}
				}

				/// Editor Tabs
				{
					const auto tab_cursor_position = 800; /// There HAS to be an alternative to spamming ImGui::SetCursorPos. Unaware atm, didn't research yet.
					ImGui::SetCursorPos(ImVec2(tab_cursor_position, 130));

					if (ImGui::BeginTabBar("##editor_tabs"))
					{
						if (ImGui::BeginTabItem("Pure Mode"))
						{
							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::Checkbox("Ignore Whitelist", &current_profile->data.pure_mode.allow_non_default_scripts);

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::Checkbox("Geometric Changes", &current_profile->data.pure_mode.allow_geometric_changes);

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::Checkbox("Silence", &current_profile->data.pure_mode.silence);

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::Checkbox("VAC Tools", &current_profile->data.pure_mode.allow_vac_tools);

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::Checkbox("HWID Spoofer", &current_profile->data.pure_mode.hwid_spoofer);

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::Checkbox("Always Update", &current_profile->data.pure_mode.always_update);

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::Checkbox("Driverless (Veteran Only)", &current_profile->data.pure_mode.driverless);

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::InputText("Zombie", &current_profile->data.pure_mode.zombie);

							ImGui::SetCursorPosX(tab_cursor_position);
							if (ImGui::Button("Set", ImVec2(100, 20)))
							{
								/// Update profile information.
								g_configuration.save_profile();
								g_logger.add("Pure Mode information updated.");
							}

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Whitelist"))
						{
							/*
								Whitelist is not profile based and should not be such.
								Setting the value for whitelist will change the whitelist.txt file.
							*/
							static auto whitelist_raw = g_moonlight.get_whitelist_raw();

							ImGui::SetCursorPosX(tab_cursor_position);
							ImGui::InputTextMultiline("##whitelist_editor", &whitelist_raw, ImVec2(0, 430));

							ImGui::SetCursorPosX(tab_cursor_position);
							if (ImGui::Button("Refresh", ImVec2(100, 20)))
							{
								/// Refresh whitelist string.
								whitelist_raw = g_moonlight.get_whitelist_raw();
								g_logger.add("Refreshed raw whitelist.txt.");
							}

							ImGui::SameLine();

							if (ImGui::Button("Set", ImVec2(100, 20)))
							{
								/// Directly set whitelist information as it's not profile based.
								g_moonlight.set_whitelist_raw( whitelist_raw );
								g_logger.add("Set new whitelist.txt.");
							}

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Profile"))
						{
							/// Set Moonlight binary.
							{
								ImGui::SetCursorPosX(tab_cursor_position);
								ImGui::Text("Moonlight Binary (Location of fantasy.moonlight.exe):");

								ImGui::SetNextItemWidth(400);
								ImGui::SetCursorPosX(tab_cursor_position);
								ImGui::InputText("##MoonlightBinary", &current_profile->path);
							}

							/// Allow creation of new profiles.
							{
								static std::string new_profile_name;

								ImGui::SetCursorPosX(tab_cursor_position);
								ImGui::Text("Create new profile:");

								ImGui::SetNextItemWidth(400);
								ImGui::SetCursorPosX(tab_cursor_position);
								ImGui::InputText("##NewProfile", &new_profile_name);

								ImGui::SetCursorPosX(tab_cursor_position);

								/// Create the profile.
								if (ImGui::Button("Create Profile", ImVec2(120, 20)))
								{
									if (!new_profile_name.empty())
									{
										/// Add .json to profile name.
										new_profile_name += ".json";

										/// Add to database.
										profile_information temp;
										temp.name = new_profile_name;
										g_configuration.profiles.push_back( std::make_unique< profile_information >( temp ) );

										/// Set current profile to new profile.
										g_configuration.current_profile = new_profile_name;

										/// Save new profile.
										g_configuration.save_profile();

										/// Display and log.
										auto pfmt = std::format("Profile \"{}\" created. You are now on your new profile.", new_profile_name);
										g_logger.add(pfmt);
										MessageBoxA(NULL, pfmt.c_str(), "FSM", MB_OK);

										/// Erase buffer
										new_profile_name.clear();
									}
									else g_logger.add("Invalid profile name.");
								}
							}

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Metadata"))
						{
							/// Loop through all the scripts.
							for (auto& i : g_moonlight.script_database)
							{
								ImGui::SetCursorPosX(tab_cursor_position);

								/// Have to create a child here otherwise the TreeNodes will create to infinity.
								if (ImGui::BeginChild("##meta_data", ImVec2(465, 455)))
								{
									if (ImGui::TreeNode(i.name.c_str()))
									{
										ImGui::InputText("Description", &i.description);
										ImGui::Combo("Class", &i.script_class, class_options, IM_ARRAYSIZE(class_options), IM_ARRAYSIZE(class_options));

										if (ImGui::Button("Open in Editor", ImVec2(130, 20)))
										{
											/// Open script with default editor.
											ShellExecuteA(NULL, "open", i.path.c_str(), NULL, NULL, SW_SHOWNORMAL);
										}

										ImGui::TreePop();
									}
									ImGui::EndChild();
								}
							}

							ImGui::SetCursorPosX(tab_cursor_position);
							if (ImGui::Button("Set", ImVec2(100, 20)))
							{
								/// Update meta_data.txt.
								g_moonlight.set_scripts_metadata(g_moonlight.script_database);
								g_logger.add("Set new script metadata.");
							}

							

							
							ImGui::EndTabItem();
						}
					}
				}
				
				/// Logging display.
				{
					ImGui::SetCursorPos(ImVec2(5, 630));
					ImGui::PushItemWidth(1000);
					if (ImGui::BeginTable("Logs", 1, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_ScrollY, ImVec2(1260, 130), 50))
					{
						ImGui::TableNextColumn();
						ImGui::Text("Log");
						
						for (auto& i : g_logger.logs)
						{
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							ImGui::Text(i.format.c_str());
						}

						ImGui::EndTable();
					}
					ImGui::PopItemWidth();
				}
				
				ImGui::End();
			}
			
			g_gui.end_frame();
		}
	}
	catch (const std::exception& e)
	{
		/*
			I used std::runtime_error carelessly throughout the source code. 
			Catching errors will simply log the issue however, the software will crash.

			This wasn't intentional but at the same time, if you end up getting a runtime error, the program
			should break in general.
		*/
		g_logger.add(e.what());
	}
	return 1;
}
