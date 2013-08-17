#include "JgeStdafx.h"
#include "JgeWindow.h"

#pragma comment(lib, "JgeFoundation.lib")

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevHInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	jge::Window wnd;
	wnd.create(hInstance, 0, 0, 800, 600, true, L"asdfasdf");
	return 0;
}