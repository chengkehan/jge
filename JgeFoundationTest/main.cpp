#include "JgeStdafx.h"
#include "JgeWindow.h"

#pragma comment(lib, "JgeFoundation.lib")

jge::Window wnd;

LRESULT CALLBACK wmClose(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	if(wparam == VK_ESCAPE)
	{
		DestroyWindow(hwnd);
	}
	else if(wparam == VK_SPACE)
	{
		wnd.setSize(1000, 700);
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
	wnd.create(hInstance, -1, -1, 800, 600, true, L"asdfasdfsdf");
	jge::Window::registerWndProc(wnd.getHWnd(), WM_KEYDOWN, wmClose);
	jge::Window::registerWndProc(wnd.getHWnd(), WM_DESTROY, wmDestroy);
	wnd.setTitle(L"WOW");
	wnd.run();
	return 0;
}