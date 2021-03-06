#include "JgeStdafx.h"
#include "JgeWindow.h"
#include "JgeString.h"
#include "JgeScreen.h"

int jge::Window::s_wndCount = 0;
jge::Window::HWndMap* jge::Window::s_lpMsgMap = null;

jge::Window::Window():
	m_hInstance(null), m_hWnd(null), m_lpTitle(null), m_lpClassName(null), 
	m_windowWidth(0), m_windowHeight(0), m_windowd(false), m_running(false)
{
	// Do nothing
}

jge::Window::~Window()
{
	release();
}

bool jge::Window::init(HINSTANCE hInstance, int windowX, int windowY, uint windowWidth, uint windowHeight, bool windowd, wchar_t* lpTitle)
{
	if(m_hInstance != null || m_hWnd != null || hInstance == null || windowWidth == 0 || windowHeight == 0 || m_running)
	{
		return false;
	}

	m_hInstance = hInstance;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_windowd = windowd;
	if(lpTitle == null)
	{
		jgewcs2(m_lpTitle, "JgeWindow", s_wndCount);
	}
	else
	{
		m_lpTitle = jgewcsclone(lpTitle);
	}
	jgewcs2(m_lpClassName, "JgeWindow", s_wndCount);

	++s_wndCount;

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = jge::Window::wndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(null, IDI_APPLICATION);
	wc.hCursor = LoadCursor(null, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = null;
	wc.lpszClassName = m_lpClassName;

	if(!RegisterClass(&wc))
	{
		release();
		return false;
	}

	if(m_windowd)
	{
		uint width, height;
		int x, y;
		jge::Window::getWindowAdjustedSize(windowWidth, windowHeight, &x, &y, &width, &height);

		RECT rect;
		rect.left = windowX == -1 ? x : 0;
		rect.top = windowY == -1 ? y : 0;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
		m_hWnd = CreateWindowEx(0, m_lpClassName, m_lpTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU /*| WS_THICKFRAME*/ | WS_MINIMIZEBOX /*| WS_MAXIMIZEBOX*/, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, null, null, hInstance, null);
	}
	else
	{
		m_hWnd = CreateWindowEx(WS_EX_TOPMOST, m_lpClassName, m_lpTitle, WS_POPUP | WS_VISIBLE, 0, 0, windowWidth, windowHeight, null, null, hInstance, null);
	}

	if(m_hWnd == null)
	{
		release();
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

bool jge::Window::setTitle(const wchar_t* lpTitle)
{
	if(m_hInstance == null || m_hWnd == null)
	{
		return false;
	}

	jgewcsfree(m_lpTitle);
	m_lpTitle = jgewcsclone(lpTitle);
	SetWindowText(m_hWnd, m_lpTitle);
	return true;
}

bool jge::Window::setSize(uint windowWidth, uint windowHeight)
{
	if(m_hInstance == null || m_hWnd == null)
	{
		return false;
	}

	if(windowWidth == m_windowWidth && windowHeight == m_windowHeight)
	{
		return false;
	}

	if(!m_windowd && !jge::ScreenResolution::checkScreenResolution(windowWidth, windowHeight))
	{
		return false;
	}

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	SetWindowPos(m_hWnd, null, 0, 0, m_windowWidth, m_windowHeight, SWP_NOZORDER | SWP_NOMOVE);
	return true;
}

void jge::Window::release()
{
	unregisterWindowAllMessages(this);
	m_hInstance = null;
	m_hWnd = null;
	m_windowd = false;
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_running = false;
	jgewcsfree(m_lpTitle);
	jgewcsfree(m_lpClassName);
	jgewcsfree(m_lpClassName);
}

bool jge::Window::registerWindowMessage(jge::Window* lpWindow, uint msg, WNDPROC wndProc)
{
	if(lpWindow == null || wndProc == null)
	{
		return false;
	}

	if(s_lpMsgMap == null)
	{
		jgeNew(s_lpMsgMap, jge::Window::HWndMap);
	}

	jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->find(lpWindow);
	jge::Window::WndProcMap* lpWndProcMap = null;
	if(hWndIter == s_lpMsgMap->end())
	{
		jgeNew(lpWndProcMap, jge::Window::WndProcMap);
		s_lpMsgMap->insert(jge::Window::HWndMap::value_type(lpWindow, lpWndProcMap));
	}
	else
	{
		lpWndProcMap = hWndIter->second;
	}

	if(lpWndProcMap == null)
	{
		return false;
	}
	
	(*lpWndProcMap)[msg] = wndProc;
	//std::pair<jge::Window::WndProcMap::iterator, bool> result = lpWndProcMap->insert(jge::Window::WndProcMap::value_type(msg, wndProc));
	return true;
}

bool jge::Window::unregisterWindowMessage(jge::Window* lpWindow, uint msg)
{
	if(lpWindow == null || s_lpMsgMap == null)
	{
		return false;
	}

	jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->find(lpWindow);
	if(hWndIter == s_lpMsgMap->end())
	{
		return false;
	}

	jge::Window::WndProcMap* lpWndProcMap = hWndIter->second;
	if(lpWndProcMap == null)
	{
		return false;
	}

	jge::Window::WndProcMap::size_type numErase = lpWndProcMap->erase(msg);
	if(numErase > 0)
	{
		if(lpWndProcMap->empty())
		{
			jgeDelete(lpWndProcMap);
			s_lpMsgMap->erase(lpWindow);
		}
		return true;
	}
	return false;
}

bool jge::Window::unregisterWindowAllMessages(jge::Window* lpWindow)
{
	if(lpWindow == null || s_lpMsgMap == null)
	{
		return false;
	}

	jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->find(lpWindow);
	if(hWndIter == s_lpMsgMap->end())
	{
		return false;
	}

	jge::Window::WndProcMap* lpWndProcMap = hWndIter->second;
	s_lpMsgMap->erase(lpWindow);
	jgeDelete(lpWndProcMap);
	return true;
}

void jge::Window::getWindowAdjustedSize(uint widthSrc, uint heightSrc, int* lpXResult, int* lpYResult, uint* lpWidthResult, uint* lpHeightResult)
{
	uint widthResult = widthSrc + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	uint heightResult = heightSrc + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	if(lpWidthResult != null)
	{
		*lpWidthResult = widthResult;
	}
	if(lpHeightResult != null)
	{
		*lpHeightResult = heightResult;
	}
	if(lpXResult != null)
	{
		*lpXResult = (int)((GetSystemMetrics(SM_CXSCREEN) - widthResult) * 0.5f);
	}
	if(lpYResult != null)
	{
		*lpYResult = (int)((GetSystemMetrics(SM_CYSCREEN) - heightResult) * 0.5f);
	}
}

LRESULT CALLBACK jge::Window::wndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam)
{
	if(s_lpMsgMap != null)
	{
		for (jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->begin(); hWndIter != s_lpMsgMap->end(); ++hWndIter)
		{
			Window* lpWindow = hWndIter->first;
			if(lpWindow->getHWnd() == hWnd)
			{
				jge::Window::WndProcMap* lpWndProcMap = hWndIter->second;
				jge::Window::WndProcMap::iterator wndProcIter = lpWndProcMap->find(msg);
				if(wndProcIter != lpWndProcMap->end())
				{
					WNDPROC wndProcCallback = wndProcIter->second;
					return wndProcCallback(hWnd, msg, wParam, lParam);
				}
			}
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}