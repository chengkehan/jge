#pragma once

#include <map>
#include "JgeCommon.h"

namespace jge
{
	class JGE_DLL Window
	{
	public:
		Window();
		~Window();

		bool create(HINSTANCE hInstance, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true, wchar_t* lpTitle = null, wchar_t* lpClassName = null);
		void destroy();

		static bool registerWndProc(HWND hWnd, UINT msg, WNDPROC wndProc);
		static bool unregisterWndProc(HWND hWnd, UINT msg);

	private:
		typedef std::map<UINT, WNDPROC> WndProcMap;
		typedef std::map<HWND, WndProcMap*> HWndMap;

		Window(const Window& value);
		Window& operator=(const Window& value);

		HWND m_hWnd;
		HINSTANCE m_hInstance;
		uint m_windowWidth;
		uint m_windowHeight;
		bool m_windowd;
		wchar_t* m_lpTitle;
		wchar_t* m_lpClassName;
		
		inline HWND getHWnd() const { return m_hWnd; }
		inline HINSTANCE getHInstance() const { return m_hInstance; }
		inline uint getWindowWidth() const { return m_windowWidth; }
		inline uint getWindowHeight() const { return m_windowHeight; }
		inline bool getWindowd() const { return m_windowd; }
		inline const wchar_t* getTitle() const { return m_lpTitle; }

		static HWndMap* s_lpMsgMap;
		static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}
