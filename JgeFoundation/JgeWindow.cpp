#include "JgeStdafx.h"
#include "JgeWindow.h"
#include "JgeMemory.h"

jge::Window::HWndMap* jge::Window::s_lpMsgMap = null;

jge::Window::Window():
	m_hInstance(null), m_hWnd(null), m_lpTitle(null), m_lpClassName(null), 
	m_windowWidth(0), m_windowHeight(0), m_windowd(false)
{
	// Do nothing
}

jge::Window::~Window()
{
	unregisterAllWndProc(m_hWnd);
}

bool jge::Window::create(HINSTANCE hInstance, uint windowWidth, uint windowHeight, bool windowd, wchar_t* lpTitle, wchar_t* lpClassName)
{
	if(m_hInstance != null || m_hWnd != null || hInstance == null || windowWidth == 0 || windowHeight == 0)
	{
		return false;
	}

	m_hInstance = hInstance;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_windowd = windowd;
	m_lpTitle = lpTitle;
	m_lpClassName = lpClassName;

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = jge::Window::wndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(null, IDI_APPLICATION);
	wc.hCursor = LoadCursor(null, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = null;
	wc.lpszClassName = lpClassName;

	return false;
}

void jge::Window::destroy()
{

}

bool jge::Window::registerWndProc(HWND hWnd, UINT msg, WNDPROC wndProc)
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

bool jge::Window::unregisterWndProc(HWND hWnd, UINT msg)
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

bool jge::Window::unregisterAllWndProc(HWND hWnd)
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

LRESULT CALLBACK jge::Window::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
				WNDPROC wndProcCallback = wndProcIter->second;
				return wndProcCallback(hWnd, msg, wParam, lParam);
			}
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}