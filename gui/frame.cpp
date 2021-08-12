/*
	@title
		frame.cpp

	@author
		typedef
*/
/// gui-information
#include "gui.h"

auto gui_information::start_frame() -> bool
{
	MSG msg;
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) return false;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return true;
}

auto gui_information::end_frame() -> void
{
	ImGui::Render();

	ImVec4 background_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const float background_color_alpha[4] =
	{
		background_color.x * background_color.w,
		background_color.y * background_color.w,
		background_color.z * background_color.w,
		background_color.w
	};

	device_context->OMSetRenderTargets(1, &main_render_target_view, NULL);
	device_context->ClearRenderTargetView(main_render_target_view, background_color_alpha);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	swap_chain->Present(1, 0);
}