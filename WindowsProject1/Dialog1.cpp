#include "Dialog1.h"
#include "resource.h"
#include "ListView.h"
template<>INT_PTR Dialog1::HandleCommand<IDCANCEL>(HWND hDlg, WORD wNotify, HWND hCtrl) noexcept
{
	EndDialog(hDlg, IDCANCEL);
	return true;
}
template<>INT_PTR Dialog1::HandleMessage<WM_COMMAND>(HWND hDlg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (LOWORD(wParam)) {
#define	Dispatch(id) case id: return HandleCommand<id>(hDlg, HIWORD(wParam), HWND(lParam));
	Dispatch(IDCANCEL);
#undef	Dispatch
	default:
		return false;
	}
}

#include <sstream>
#include <iomanip>
static void UpdateTimestamp(HWND hListView, int iItem)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	const auto timestamp = (std::basic_ostringstream<TCHAR>() << std::setfill(TEXT('0'))
		<< std::setw(4) << st.wYear   << TEXT('/')
		<< std::setw(2) << st.wMonth  << TEXT('/')
		<< std::setw(2) << st.wDay    << TEXT(' ')
		<< std::setw(2) << st.wHour   << TEXT(':')
		<< std::setw(2) << st.wMinute << TEXT(':')
		<< std::setw(2) << st.wSecond).str();
	LVITEM m
	{ /*UINT   mask       */LVIF_TEXT
	, /*int    iItem      */iItem
	, /*int    iSubItem   */1
	, /*UINT   state      */0
	, /*UINT   stateMask  */0
	, /*LPTSTR pszText    */const_cast<LPTSTR>(timestamp.c_str())
	, /*int    cchTextMax */0
	, /*int    iImage     */0
	, /*LPARAM lParam     */0
	};
	ListView_SetItem(hListView, &m);
}


template<>INT_PTR Dialog1::HandleMessage<WM_NOTIFY>(HWND hDlg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (wParam == IDC_LIST1) {
		if (LPNMHDR(lParam)->code == LVN_ENDLABELEDIT) {
			UpdateTimestamp(LPNMHDR(lParam)->hwndFrom, reinterpret_cast<NMLVDISPINFO*>(lParam)->item.iItem);
			SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, true);
		}
		return true;
	}
	else
		return false;
}
INT_PTR Dialog1::DialogBoxParamW(HWND hWndParent) noexcept
{
	return /*WINUSERAPI INT_PTR WINAPI*/::DialogBoxParamW
	( /*_In_opt_ HINSTANCE hInstance     */nullptr
	, /*_In_     LPCWSTR   lpTemplateName*/MAKEINTRESOURCEW(IDD_DIALOG1)
	, /*_In_opt_ HWND      hWndParent    */nullptr
	, /*_In_opt_ DLGPROC   lpDialogFunc  */[](HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) noexcept->INT_PTR
		{
			if (const auto pThis = reinterpret_cast<Dialog1*>(GetWindowLongPtrW(hDlg, DWLP_USER))) {
				switch (message) {
#define	Dispatch(id) case id: return pThis->HandleMessage<id>(hDlg, wParam, lParam)
					Dispatch(WM_COMMAND);
					Dispatch(WM_NOTIFY);
#undef	Dispatch
					
				default:
					return false;
				}
			}
			else if (message == WM_INITDIALOG) {
				SetWindowLongPtrW(hDlg, DWLP_USER, lParam);
				const auto hListView = GetDlgItem(hDlg, IDC_LIST1);
				ListView::InsertColumns(hListView);
				const auto pListView = new ListView(WNDPROC(GetWindowLongPtrW(hListView, GWLP_WNDPROC)));
				SetWindowLongPtrW(hListView, GWLP_USERDATA, LONG_PTR(pListView));
				SetWindowLongPtrW(hListView, GWLP_WNDPROC, LONG_PTR(static_cast<WNDPROC>([](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept->LRESULT
					{
						const auto pListView = reinterpret_cast<ListView*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
						const auto result = pListView->WndProc(hWnd, message, wParam, lParam);;
						if (message == WM_NCDESTROY)
							delete pListView;
						return result;
					})));
				return true;
			}
			else
				return false;
		}
	, /*_In_     LPARAM    dwInitParam   */LPARAM(this)
	);
}

