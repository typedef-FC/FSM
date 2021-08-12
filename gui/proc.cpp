/*
	@title
		proc.cpp

	@author
		typedef
*/
#include "gui.h"

LRESULT WINAPI wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return TRUE;

	switch (msg)
	{
		case WM_SIZE:
		{
			if (g_gui.device != NULL && wParam != SIZE_MINIMIZED)
			{
				g_gui.clean_render_target();
				g_gui.swap_chain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				g_gui.create_render_target();
			}
			return FALSE;
		}
		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xfff0) == SC_KEYMENU) return FALSE;
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return FALSE;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}