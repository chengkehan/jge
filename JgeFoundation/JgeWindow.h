#pragma once

#include <map>
#include "JgeCommon.h"
#include "JgeOS.h"

namespace jge
{
	class JGE_DLL Window
	{
	public:
		Window();
		~Window();

		bool create(JgeHINSTANCE hInstance, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true, wchar_t* lpTitle = null);
		void destroy();

		static bool registerWndProc(JgeHWND hWnd, uint msg, JgeWNDPROC wndProc);
		static bool unregisterWndProc(JgeHWND hWnd, uint msg);
		static bool unregisterAllWndProc(JgeHWND hWnd);

	private:
		typedef std::map<uint, JgeWNDPROC> WndProcMap;
		typedef std::map<JgeHWND, WndProcMap*> HWndMap;

		Window(const Window& value);
		Window& operator=(const Window& value);

		JgeHWND m_hWnd;
		JgeHINSTANCE m_hInstance;
		uint m_windowWidth;
		uint m_windowHeight;
		bool m_windowd;
		wchar_t* m_lpTitle;
		wchar_t* m_lpClassName;
		
		inline JgeHWND getHWnd() const { return m_hWnd; }
		inline JgeHINSTANCE getHInstance() const { return m_hInstance; }
		inline uint getWindowWidth() const { return m_windowWidth; }
		inline uint getWindowHeight() const { return m_windowHeight; }
		inline bool getWindowd() const { return m_windowd; }
		inline const wchar_t* getTitle() const { return m_lpTitle; }

		void release();

		static int s_wndCount;
		static HWndMap* s_lpMsgMap;
		static JgeLRESULT JgeCALLBACK wndProc(JgeHWND hWnd, uint msg, JgeWPARAM wParam, JgeLPARAM lParam);
	};
}
