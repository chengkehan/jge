#include "JgeStdafx.h"
#include "JgeWindow.h"
#include "JgeMainLoop.h"
#include "Jge3D.h"

#pragma comment(lib, "JgeFoundation.lib")

jge::Window wnd;
jge::MainLoop mainLoop;
jge::Jge3D jge3D;

LRESULT CALLBACK wmClose(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	if(wparam == VK_ESCAPE)
	{
		DestroyWindow(hwnd);
		mainLoop.stop();
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
	wnd.init(hInstance, -1, -1, 800, 600, true, L"asdfasdfsdf");
	jge::Window::registerWindowMessage(&wnd, WM_KEYDOWN, wmClose);
	jge::Window::registerWindowMessage(&wnd, WM_DESTROY, wmDestroy);
	wnd.setTitle(L"WOW");
	
	mainLoop.init(&jge3D, &wnd);
	mainLoop.run();
	
	return 0;
}