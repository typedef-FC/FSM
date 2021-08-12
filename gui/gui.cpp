/*
	@title
		gui.cpp

	@author
		typedef
*/
/// gui_information
#include "gui.h"

/// global declaration
gui_information g_gui;

auto gui_information::initialize() -> void
{
	/// Register window class.
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		wnd_proc,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		(L"FSM"),
		NULL
	};
	RegisterClassExW(&wc);

	/// Create window using our registered window class.
	HWND window_handle = CreateWindowW
	(
		wc.lpszClassName,
		(L"FSM - fantasy.cat script manager"),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		1280,
		800,
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	/// Initialize Direct 3D
	if (!initialize_direct3D(window_handle))
	{
		uninitialize_direct3D();
		UnregisterClassW(wc.lpszClassName, wc.hInstance);
		throw std::runtime_error("Could not create window.");
	}

	/// Show and /render/ the window.
	ShowWindow(window_handle, SW_SHOWDEFAULT);
	UpdateWindow(window_handle);

	/// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window_handle);
	ImGui_ImplDX11_Init(device, device_context);
}

auto gui_information::initialize_direct3D(HWND window_handle) -> bool
{
	// Create swap chain data for rendering.
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = window_handle;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	unsigned int device_flags = 0;

	D3D_FEATURE_LEVEL feature_level;
	const D3D_FEATURE_LEVEL feature_level_array[2] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};

	/// Create the device for the adapter.
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, device_flags, feature_level_array, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &device, &feature_level, &device_context) != S_OK) return false;

	/// Create rendering target since we have a swap chain ready.
	create_render_target();
	return true;
}

auto gui_information::create_render_target() -> void
{
	ID3D11Texture2D* buffer;

	swap_chain->GetBuffer(0, IID_PPV_ARGS(&buffer));
	device->CreateRenderTargetView(buffer, NULL, &main_render_target_view);

	buffer->Release();
}

auto gui_information::clean_render_target() -> void
{
	if (main_render_target_view != NULL)
	{
		main_render_target_view->Release();
		main_render_target_view = NULL;
	}
}

auto gui_information::uninitialize_direct3D() -> void
{
	clean_render_target();

	if (swap_chain != NULL)
	{
		swap_chain->Release();
		swap_chain = NULL;
	}

	if (device_context != NULL)
	{
		device_context->Release();
		device_context = NULL;
	}

	if (device != NULL)
	{
		device->Release();
		device = NULL;
	}
}