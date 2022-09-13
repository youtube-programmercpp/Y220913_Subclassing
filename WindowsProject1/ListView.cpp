#include "ListView.h"
#include <tuple>

ListView::ListView(_In_ WNDPROC lpPrevWndFunc) noexcept
	: lpPrevWndFunc{ lpPrevWndFunc }
{
}
void ListView::InsertColumns(HWND hListView) noexcept
{
	int iCol = 0;
	for (const auto& r :
		{ std::make_tuple(TEXT("Memo"     ), 300)
		, std::make_tuple(TEXT("Timestamp"), 200)
		})
	{
		LVCOLUMN col
		{ /*UINT   mask      */LVCF_TEXT | LVCF_WIDTH
		, /*int    fmt       */0
		, /*int    cx        */                   std::get<1>(r) 
		, /*LPTSTR pszText   */const_cast<LPTSTR>(std::get<0>(r))
		};
		ListView_InsertColumn(hListView, iCol++, &col);
	}
}

static void delete_item(HWND hListView)
{
	const auto iSelectedItem = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
	if (iSelectedItem != -1) {
		ListView_DeleteItem(hListView, iSelectedItem);
	}
}
static void insert_item(HWND hListView)
{
	LVITEM m
	{ /*UINT   mask       */LVIF_TEXT
	, /*int    iItem      */INT_MAX
	, /*int    iSubItem   */0
	, /*UINT   state      */0
	, /*UINT   stateMask  */0
	, /*LPTSTR pszText    */const_cast<LPTSTR>(TEXT(""))
	, /*int    cchTextMax */0
	, /*int    iImage     */0
	, /*LPARAM lParam     */0
	};
	const auto iItem = ListView_InsertItem(hListView, &m);
	ListView_EditLabel(hListView, iItem);
}

LRESULT ListView::WndProc
( _In_ HWND   hWnd  
, _In_ UINT   Msg   
, _In_ WPARAM wParam
, _In_ LPARAM lParam
) noexcept
{
	if (Msg == WM_KEYDOWN) {
		switch (wParam) {
		case VK_INSERT:
			insert_item(hWnd);
			return 0;
		case VK_DELETE:
			delete_item(hWnd);
			return 0;
		}
	}
	return /*WINUSERAPI LRESULT WINAPI*/CallWindowProcW
	( /*_In_ WNDPROC lpPrevWndFunc*/lpPrevWndFunc
	, /*_In_ HWND    hWnd         */hWnd         
	, /*_In_ UINT    Msg          */Msg          
	, /*_In_ WPARAM  wParam       */wParam       
	, /*_In_ LPARAM  lParam       */lParam       
	);
}
