#include "Dialog1.h"

int APIENTRY wWinMain
( _In_     HINSTANCE hInstance
, _In_opt_ HINSTANCE hPrevInstance
, _In_     LPWSTR    lpCmdLine
, _In_     int       nCmdShow
)
{
	Dialog1 dlg;
	(void)dlg.DialogBoxParamW(nullptr);
}
