#include "JgeStdafx.h"
#include "JgeWindow.h"
#include "JgeMemory.h"
#include "JgeString.h"

int jge::Window::s_wndCount = 0;
jge::Window::HWndMap* jge::Window::s_lpMsgMap = null;

jge::Window::Window():
	m_hInstance(null), m_hWnd(null), m_lpTitle(null), m_lpClassName(null), 
	m_windowWidth(0), m_windowHeight(0), m_windowd(false)
{
	// Do nothing
}

jge::Window::~Window()
{
	release();
}

bool jge::Window::create(jgeHINSTANCE hInstance, int windowX, int windowY, uint windowWidth, uint windowHeight, bool windowd, wchar_t* lpTitle)
{
	if(m_hInstance != null || m_hWnd != null || hInstance == null || windowWidth == 0 || windowHeight == 0)
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

	jgeWNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = jgeCS_HREDRAW | jgeCS_VREDRAW;
	wc.lpfnWndProc = jge::Window::wndProc;
	wc.hInstance = hInstance;
	wc.hIcon = jgeLoadIcon(null, jgeIDI_APPLICATION);
	wc.hCursor = jgeLoadCursor(null, jgeIDC_ARROW);
	wc.hbrBackground = (jgeHBRUSH)GetStockObject(jgeWHITE_BRUSH);
	wc.lpszMenuName = null;
	wc.lpszClassName = m_lpClassName;

	if(!jgeRegisterClass(&wc))
	{
		release();
		return false;
	}

	if(m_windowd)
	{
		uint width, height;
		int x, y;
		jgeGetWindowAdjustedSize(windowWidth, windowHeight, &x, &y, &width, &height);

		jgeRECT rect;
		rect.left = windowX == -1 ? x : 0;
		rect.top = windowY == -1 ? y : 0;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
		m_hWnd = jgeCreateWindowEx(0, m_lpClassName, m_lpTitle, jgeWS_POPUP | jgeWS_CAPTION | jgeWS_MINIMIZEBOX | jgeWS_VISIBLE, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, null, null, hInstance, null);
	}
	else
	{
		m_hWnd = jgeCreateWindowEx(jgeWS_EX_TOPMOST, m_lpClassName, m_lpTitle, jgeWS_POPUP | jgeWS_VISIBLE, 0, 0, windowWidth, windowHeight, null, null, hInstance, null);
	}

	if(m_hWnd == null)
	{
		release();
		return false;
	}

	ShowWindow(m_hWnd, jgeSW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

void jge::Window::destroy()
{

}

void jge::Window::release()
{
	unregisterAllWndProc(m_hWnd);
	m_hInstance = null;
	m_hWnd = null;
	m_windowd = false;
	m_windowWidth = 0;
	m_windowHeight = 0;
	jgewcsfree(m_lpTitle);
	jgewcsfree(m_lpClassName);
}

bool jge::Window::registerWndProc(jgeHWND hWnd, uint msg, jgeWNDPROC wndProc)
{
	if(hWnd == null || wndProc == null)
	{
		return false;
	}

	if(s_lpMsgMap == null)
	{
		jgeNew(s_lpMsgMap, jge::Window::HWndMap);
	}

	jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->find(hWnd);
	jge::Window::WndProcMap* lpWndProcMap = null;
	if(hWndIter == s_lpMsgMap->end())
	{
		jgeNew(lpWndProcMap, jge::Window::WndProcMap);
		s_lpMsgMap->insert(jge::Window::HWndMap::value_type(hWnd, lpWndProcMap));
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

bool jge::Window::unregisterWndProc(jgeHWND hWnd, uint msg)
{
	if(hWnd == null || s_lpMsgMap == null)
	{
		return false;
	}

	jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->find(hWnd);
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
			s_lpMsgMap->erase(hWnd);
		}
		return true;
	}
	return false;
}

bool jge::Window::unregisterAllWndProc(jgeHWND hWnd)
{
	if(hWnd == null || s_lpMsgMap == null)
	{
		return false;
	}

	jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->find(hWnd);
	if(hWndIter == s_lpMsgMap->end())
	{
		return false;
	}

	jge::Window::WndProcMap* lpWndProcMap = hWndIter->second;
	s_lpMsgMap->erase(hWnd);
	jgeDelete(lpWndProcMap);
	return true;
}

JgeLRESULT jgeCALLBACK jge::Window::wndProc(jgeHWND hWnd, uint msg, jgeWPARAM wParam, jgeLPARAM lParam)
{
	if(s_lpMsgMap != null)
	{
		jge::Window::HWndMap::iterator hWndIter = s_lpMsgMap->find(hWnd);
		if(hWndIter != s_lpMsgMap->end())
		{
			jge::Window::WndProcMap* lpWndProcMap = hWndIter->second;
			jge::Window::WndProcMap::iterator wndProcIter = lpWndProcMap->find(msg);
			if(wndProcIter != lpWndProcMap->end())
			{
				jgeWNDPROC wndProcCallback = wndProcIter->second;
				return wndProcCallback(hWnd, msg, wParam, lParam);
			}
		}
	}

	return jgeDefWindowProc(hWnd, msg, wParam, lParam);
}