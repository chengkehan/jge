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

	IDirect3DDevice9* lpd3dd = m_lpJge3D->getD3DDevice();

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
			lpd3dd->Clear(0, null, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0);
			lpd3dd->BeginScene();
			// Draw calls
			lpd3dd->EndScene();
			// Calculate logic,ai,physics,network,etc.
			lpd3dd->Present(null, null, null, null);
		}
	}

	return true;
}

void jge::MainLoop::stop()
{
	m_running = false;
}

