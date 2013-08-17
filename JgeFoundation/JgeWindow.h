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

		bool create(jgeHINSTANCE hInstance, int windowX = -1, int windowY = -1, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true, wchar_t* lpTitle = null);
		void destroy();

		static bool registerWndProc(jgeHWND hWnd, uint msg, jgeWNDPROC wndProc);
		static bool unregisterWndProc(jgeHWND hWnd, uint msg);
		static bool unregisterAllWndProc(jgeHWND hWnd);

	private:
		typedef std::map<uint, jgeWNDPROC> WndProcMap;
		typedef std::map<jgeHWND, WndProcMap*> HWndMap;

		Window(const Window& value);
		Window& operator=(const Window& value);

		jgeHWND m_hWnd;
		jgeHINSTANCE m_hInstance;
		uint m_windowWidth;
		uint m_windowHeight;
		bool m_windowd;
		wchar_t* m_lpTitle;
		wchar_t* m_lpClassName;
		
		inline jgeHWND getHWnd() const { return m_hWnd; }
		inline jgeHINSTANCE getHInstance() const { return m_hInstance; }
		inline uint getWindowWidth() const { return m_windowWidth; }
		inline uint getWindowHeight() const { return m_windowHeight; }
		inline bool getWindowd() const { return m_windowd; }
		inline const wchar_t* getTitle() const { return m_lpTitle; }

		void release();

		static int s_wndCount;
		static HWndMap* s_lpMsgMap;
		static JgeLRESULT jgeCALLBACK wndProc(jgeHWND hWnd, uint msg, jgeWPARAM wParam, jgeLPARAM lParam);
	};
}
