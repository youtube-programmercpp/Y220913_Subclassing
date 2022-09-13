#pragma once
#include <Windows.h>
class Dialog1
{
	template<WORD id>INT_PTR HandleCommand(HWND hDlg, WORD wNotify, HWND hCtrl) noexcept;
	template<unsigned message>INT_PTR HandleMessage(HWND hDlg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	INT_PTR DialogBoxParamW(HWND hWndParent) noexcept;
};

