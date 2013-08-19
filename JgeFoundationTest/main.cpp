#include "JgeStdafx.h"
#include "JgeWindow.h"

#pragma comment(lib, "JgeFoundation.lib")

LRESULT CALLBACK wmClose(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	if(wparam == VK_ESCAPE)
	{
		DestroyWindow(hwnd);
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK wmDestroy(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevHInstance, LPSTR lpCmdLine, int nCmdShow)
{
	jge::Window wnd;
	wnd.create(hInstance);
	jge::Window::registerWndProc(wnd.getHWnd(), WM_KEYDOWN, wmClose);
	jge::Window::registerWndProc(wnd.getHWnd(), WM_DESTROY, wmDestroy);
	wnd.run();
	return 0;
}