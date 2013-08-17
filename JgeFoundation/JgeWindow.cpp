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

bool jge::Window::create(JgeHINSTANCE hInstance, uint windowWidth, uint windowHeight, bool windowd, wchar_t* lpTitle)
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
	jgewcs2(m_lpClassName, "JgeWindow", ++s_wndCount);

	JgeWNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = JgeCS_HREDRAW | JgeCS_VREDRAW;
	wc.lpfnWndProc = jge::Window::wndProc;
	wc.hInstance = hInstance;
	wc.hIcon = JgeLoadIcon(null, JgeIDI_APPLICATION);
	wc.hCursor = JgeLoadCursor(null, JgeIDC_ARROW);
	wc.hbrBackground = (JgeHBRUSH)GetStockObject(JgeWHITE_BRUSH);
	wc.lpszMenuName = null;
	wc.lpszClassName = m_lpClassName;

	if(!JgeRegisterClass(&wc))
	{
		return false;
	}

	return false;
}

void jge::Window::destroy()
{

}

void jge::Window::release()
{
	unregisterAllWndProc(m_hWnd);
	jgewcsfree(m_lpClassName);
}

bool jge::Window::registerWndProc(JgeHWND hWnd, uint msg, JgeWNDPROC wndProc)
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

bool jge::Window::unregisterWndProc(JgeHWND hWnd, uint msg)
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

bool jge::Window::unregisterAllWndProc(JgeHWND hWnd)
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

JgeLRESULT JgeCALLBACK jge::Window::wndProc(JgeHWND hWnd, uint msg, JgeWPARAM wParam, JgeLPARAM lParam)
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
				JgeWNDPROC wndProcCallback = wndProcIter->second;
				return wndProcCallback(hWnd, msg, wParam, lParam);
			}
		}
	}

	return JgeDefWindowProc(hWnd, msg, wParam, lParam);
}