#include "JgeStdafx.h"
#include "JgeWindow.h"

#pragma comment(lib, "JgeFoundation.lib")

jgeLRESULT jgeCALLBACK wmClose(jgeHWND hwnd, uint msg, jgeWPARAM wparam, jgeLPARAM lparam)
{
	if(wparam == VK_ESCAPE)
	{
		DestroyWindow(hwnd);
	}
	return jgeDefWindowProc(hwnd, msg, wparam, lparam);
}

jgeLRESULT jgeCALLBACK wmDestroy(jgeHWND hwnd, uint msg, jgeWPARAM wparam, jgeLPARAM lparam)
{
	PostQuitMessage(0);
	return jgeDefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevHInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	jge::Window wnd;
	wnd.create(hInstance);
	jge::Window::registerWndProc(wnd.getHWnd(), WM_KEYDOWN, wmClose);
	jge::Window::registerWndProc(wnd.getHWnd(), WM_DESTROY, wmDestroy);
	wnd.run();
	return 0;
}