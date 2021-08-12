/*
	@title
		gui.h
	
	@author
		typedef
*/
#pragma once

/// dxd311
#include <d3d11.h>

/// std::runtime_error
#include <stdexcept>

/// Dear ImGui
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_internal.h"
#include "../include/imgui/imgui_impl_dx11.h"
#include "../include/imgui/imgui_impl_win32.h"
#include "../include/imgui/imgui_stdlib.h"


/// Forward
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern LRESULT WINAPI wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct gui_information
{
	/// <summary>
	/// Direct3D Device
	/// </summary>
	ID3D11Device*			device = NULL;

	/// <summary>
	/// Direct3D Content Device for rendering commands.
	/// </summary>
	ID3D11DeviceContext*	device_context = NULL;

	/// <summary>
	/// Swap Chain
	/// </summary>
	IDXGISwapChain*			swap_chain = NULL;

	/// <summary>
	/// Render target used for resources during rendering.
	/// </summary>
	ID3D11RenderTargetView* main_render_target_view = NULL;

	/// <summary>
	/// Creates a render target for resources during rendering.
	/// </summary>
	/// <returns></returns>
	auto create_render_target() -> void;

	/// <summary>
	/// Clear our resources.
	/// </summary>
	/// <returns></returns>
	auto clean_render_target() -> void;

	/// <summary>
	/// Uninitialize our Direct3D device.
	/// </summary>
	/// <returns></returns>
	auto uninitialize_direct3D() -> void;

	/// <summary>
	/// Initialize our Direct3D Device.
	/// </summary>
	/// <param name="window_handle">Target window.</param>
	/// <returns>true if success; false otherwise.</returns>
	auto initialize_direct3D(HWND window_handle) -> bool;

	/// <summary>
	/// Initialize our GUI module.
	/// </summary>
	/// <returns></returns>
	auto initialize() -> void;

	/// <summary>
	/// Render new frame.
	/// </summary>
	/// <returns></returns>
	auto start_frame() -> bool;

	/// <summary>
	/// Set render values after new frames are  rendered.
	/// </summary>
	/// <returns></returns>
	auto end_frame() -> void;
};

extern gui_information g_gui;