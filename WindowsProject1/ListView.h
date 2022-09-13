#pragma once
#include <Windows.h>
#include <CommCtrl.h>
class ListView
{
	WNDPROC lpPrevWndFunc;
public:
	ListView(_In_ WNDPROC previous_wndproc) noexcept;
	static void InsertColumns(HWND hListView) noexcept;
	LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept;
};

