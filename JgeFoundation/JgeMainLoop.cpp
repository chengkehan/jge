#include "JgeStdafx.h"
#include "JgeMainLoop.h"
#include "JgeMemory.h"

jge::MainLoop::MainLoop():
	m_running(false), m_lpJge3D(null), m_lpWindow(null)
{
	// Do nothing
}

jge::MainLoop::~MainLoop()
{
	m_running = false;
	m_lpJge3D = null;
	m_lpWindow = null;
}

bool jge::MainLoop::init(jge::Jge3D* lpJge3D, jge::Window* lpWindow)
{
	if(m_lpJge3D != null || m_lpWindow != null || m_running || lpJge3D == null || lpWindow == null)
	{
		return false;
	}

	m_lpJge3D = lpJge3D;
	m_lpWindow = lpWindow;

	return true;
}

bool jge::MainLoop::run()
{
	if(m_lpJge3D == null || m_lpWindow == null || m_running)
	{
		return false;
	}
	m_running = true;

	MSG msg;
	jgeZeroMem(&msg, sizeof(MSG));
	while(m_running)
	{
		if(PeekMessage(&msg, null, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)	
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}

	return true;
}

void jge::MainLoop::stop()
{
	m_running = false;
}

