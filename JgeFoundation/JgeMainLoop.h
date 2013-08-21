#pragma once

#include "Jge3D.h"
#include "JgeWindow.h"

namespace jge
{
	class JGE_DLL MainLoop
	{
	public:
		MainLoop();
		~MainLoop();

		bool init(Jge3D* lpJge3D, Window* lpWindow);
		bool run();
		void stop();
		inline bool running() { return m_running; }

	private:
		MainLoop(const MainLoop& value);
		MainLoop& operator=(const MainLoop& value);

		bool m_running;
		Jge3D* m_lpJge3D;
		Window* m_lpWindow;
	};
}