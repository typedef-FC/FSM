/*
	@title
		main.cpp

	@author
		typedef

	@notes
		Includes the start, GUI render and actions.
*/

/// moonlight_information
#include "include/moonlight/moonlight.h"

/// gui_information + imgui
#include "gui/gui.h"

/// logger_information
#include "include/logger/logger.h"

/// configuration_information
#include "configuration/configuration.h"

/// std::format
#include <format>

int wmain( )
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
							if (ImGui::MenuItem("Exit"))
							{
								break;
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Tools"))
						{
							if (ImGui::BeginMenu("Profiles"))
							{
								/// Loop through all profiles.
								for (auto& i : g_configuration.profiles)
								{
									/// Create radio buttons. Only set active to one that matches the current profile.
									if (ImGui::RadioButton(i->name.c_str(), !i->name.compare(g_configuration.current_profile)) )
									{
										/// Load profile if button selected.
										g_configuration.load_profile(i);
									}
								}
								ImGui::EndMenu();
							}

							if (ImGui::MenuItem("Settings"))
							{
								/// Unused for now.
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
							ImGui::Text("Script");
							ImGui::TableNextColumn();

							ImGui::Text("Description");
							ImGui::TableNextColumn();

							ImGui::Text("Class");
							ImGui::TableNextColumn();

							ImGui::Text("Whitelisted");
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
						"Start Moonlight",
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
								/// Save our profile settings.
								g_configuration.save_profile();

								/// The user didn't set the Moonlight binary path yet. Nothing to execute.
								if (current_profile->path.empty())
								{
									g_logger.add("Missing Moonlight binary path. Please go to Profiles -> Moonlight Binary.");
									break;
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
									break;
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
								for (auto& i : std::filesystem::directory_iterator(scripts_directory) )
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
									break;
								}
							
								/*
									Log and pop up that the process started.
									TODO: Work like Mod Organizer and disable the GUI instead of a verbal request.
								*/
								auto pfmt = std::format("Process started (Process ID: {}). Please don't make further changes in FSM until Moonlight is closed.", process_info.dwProcessId);
								g_logger.add(pfmt);
								MessageBoxA(NULL, pfmt.c_str(), "FSM", MB_OK);
								break;
							}

							case 1: /// Read Patch Notes
							{
								ShellExecute(NULL, L"open", L"https://fantasy.cat/update/index.php?c=Moonlight", NULL, NULL, SW_SHOWNORMAL);
								break;
							}

							case 2: /// Open General Logs
							{
								ShellExecute(NULL, L"open", std::wstring( g_moonlight.directory.wstring() + L"\\logs\\log.txt" ).c_str(), NULL, NULL, SW_SHOWNORMAL);
								break;
							}

							case 3: /// Open Warning Logs
							{
								ShellExecute(NULL, L"open", std::wstring(g_moonlight.directory.wstring() + L"\\logs\\warning.txt").c_str(), NULL, NULL, SW_SHOWNORMAL);
								break;
							}

							case 4: /// Open Error Logs
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
								ImGui::Separator();

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
