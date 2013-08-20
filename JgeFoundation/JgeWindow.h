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

		bool create(HINSTANCE hInstance, int windowX = -1, int windowY = -1, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true, wchar_t* lpTitle = null);
		bool run();
		void stop();

		inline HWND getHWnd() const { return m_hWnd; }
		inline HINSTANCE getHInstance() const { return m_hInstance; }
		inline uint getWindowWidth() const { return m_windowWidth; }
		inline uint getWindowHeight() const { return m_windowHeight; }
		inline bool getWindowd() const { return m_windowd; }
		inline const wchar_t* getTitle() const { return m_lpTitle; }
		bool setTitle(const wchar_t* lpTitle);
		bool setSize(uint windowWidth, uint windowHeight);

		static bool registerWindowMessage(Window* lpWindow, uint msg, WNDPROC wndProc);
		static bool unregisterWindowMessage(Window* lpWindow, uint msg);
		static bool unregisterWindowAllMessages(Window* lpWindow);

	private:
		typedef std::map<uint, WNDPROC> WndProcMap;
		typedef std::map<Window*, WndProcMap*> HWndMap;

		Window(const Window& value);
		Window& operator=(const Window& value);

		HWND m_hWnd;
		HINSTANCE m_hInstance;
		uint m_windowWidth;
		uint m_windowHeight;
		bool m_windowd;
		wchar_t* m_lpTitle;
		wchar_t* m_lpClassName;
		bool m_running;

		void release();

		static int s_wndCount;
		static HWndMap* s_lpMsgMap;
		static LRESULT CALLBACK wndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
	};
}
