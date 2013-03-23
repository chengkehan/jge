#include "JGE3D.h"

JGE_SINGLETON_IMPLEMENTS(JGE3D)

jge3d::WinProcCallbackMap jge3d::winProcCallbacks;

LRESULT CALLBACK jge3d::winProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_DESTROY:
		{
			if(JGE3D::getInstance()->m_wmDestroyCallback == null || JGE3D::getInstance()->m_wmDestroyCallback())
			{
				jgeWin32Exit();
			}
			break;
		}
	}

	for (jge3d::WinProcCallbackMap::iterator iter = jge3d::winProcCallbacks.begin(); iter != jge3d::winProcCallbacks.end(); ++iter)
	{
		if(iter->first == msg && iter->second != null)
		{
			iter->second(hWnd, msg, wparam, lparam);
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}

JGE3D::JGE3D()
{
	m_hWnd = null;
	m_hInstance = null;
	m_lpd3dd = null;
	m_windowWidth = 0; m_windowHeight = 0;
	m_windowd = false;
	m_init = false;
	m_running = false;
	m_setupCallback = null;
	m_releaseCallback = null;
	m_frameCallback = null;
	m_wmDestroyCallback = null;
	m_wmEscapeKeyDownCallback = null;
	m_deviceLoseCallback = null;
	m_deviceLoseResetCallback = null;
}

JGE3D::~JGE3D()
{
	jgeReleaseCom(m_lpd3dd);
}

bool JGE3D::init(HINSTANCE hInstance, int windowX, int windowY, uint windowWidth, uint windowHeight, 
	bool windowed, bool systemMenu)
{
	if(m_init)
	{
		return true;
	}

	m_hInstance = hInstance;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	setFPS(60);

	// Create window
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = jge3d::winProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(null, IDI_APPLICATION);
	wc.hCursor = LoadCursor(null, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = null;
	wc.lpszClassName = L"jcd3dApp";

	if(!RegisterClass(&wc))
	{
		jgeMessageBoxError("RegiserClass Failed");
		return false;
	}

	HWND hwnd = null;
	if(windowed)
	{
		uint width, height;
		jgeWin32GetWindowAdjustedSize(windowWidth, windowHeight, &width, &height);

		RECT rect;
		rect.left = windowX == -1 ? (LONG)((GetSystemMetrics(SM_CXSCREEN) - width) * 0.5f) : 0;
		rect.top = windowY == -1 ? (LONG)((GetSystemMetrics(SM_CYSCREEN) - height) * 0.5f) : 0;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
		hwnd = CreateWindowEx(0, L"jcd3dApp", L"jcd3dApp", WS_POPUP | WS_CAPTION | (systemMenu ? WS_SYSMENU : 0) | WS_MINIMIZEBOX | WS_VISIBLE, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, null, null, hInstance, null);
	}
	else
	{
		hwnd = CreateWindowEx(WS_EX_TOPMOST, L"jcd3dApp", L"jcd3dApp", WS_POPUP | WS_VISIBLE, 0, 0, windowWidth, windowHeight, null, null, hInstance, null);
	}
	if(!hwnd)
	{
		jgeMessageBoxError("CreateWindow Failed");
		return false;
	}

	m_hWnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// init D3D
	LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(!d3d9)
	{
		jgeMessageBoxError("Direct3DCreate9 Failed");
		return false;
	}

	D3DCAPS9 caps;
	int vp;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	m_presentParams.BackBufferWidth = windowWidth;
	m_presentParams.BackBufferHeight = windowHeight;
	m_presentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_presentParams.BackBufferCount = 1;
	m_presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_presentParams.MultiSampleQuality = 0;
	m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_presentParams.hDeviceWindow = hwnd;
	m_presentParams.Windowed = windowed;
	m_presentParams.EnableAutoDepthStencil = true;
	m_presentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_presentParams.Flags = 0;
	m_presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &m_presentParams, &m_lpd3dd);
	if(FAILED(hr))
	{
		d3d9->Release();
		jgeMessageBoxError("CreateDevice Failed");
		return false;
	}

	if(m_setupCallback != null)
	{
		if(!m_setupCallback())
		{
			d3d9->Release();
			if(m_releaseCallback != null)
			{
				m_releaseCallback();
			}
			jgeMessageBoxError("jcd3d_setup Failed");
			return false;
		}
	}

	d3d9->Release();
	m_init = true;

	return true;
}

bool JGE3D::initManual(HINSTANCE hInstance, HWND hwnd, const D3DVIEWPORT9* lpViewPort)
{
	if(m_init)
	{
		return true;
	}

	m_hInstance = hInstance;
	m_hWnd = hwnd;
	m_viewPort = *lpViewPort;

	setFPS(60);

	// init D3D
	LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(!d3d9)
	{
		jgeMessageBoxError("Direct3DCreate9 Failed");
		return false;
	}

	D3DCAPS9 caps;
	int vp;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	m_presentParams.BackBufferWidth = m_viewPort.Width;
	m_presentParams.BackBufferHeight = m_viewPort.Height;
	m_presentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_presentParams.BackBufferCount = 1;
	m_presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_presentParams.MultiSampleQuality = 0;
	m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_presentParams.hDeviceWindow = hwnd;
	m_presentParams.Windowed = true;
	m_presentParams.EnableAutoDepthStencil = true;
	m_presentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_presentParams.Flags = 0;
	m_presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if(FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &m_presentParams, &m_lpd3dd)))
	{
		d3d9->Release();
		jgeMessageBoxError("CreateDevice Failed");
		return false;
	}
	if(!setProjectionPerspectiveTransform(m_lpd3dd, m_viewPort.Width, m_viewPort.Height))
	{
		d3d9->Release();
		jgeMessageBoxError("Projection Perspective Transform Failed");
		return false;
	}

	if(m_setupCallback != null)
	{
		if(!m_setupCallback())
		{
			d3d9->Release();
			if(m_releaseCallback != null)
			{
				m_releaseCallback();
			}
			jgeMessageBoxError("jcd3d_setup Failed");
			return false;
		}
	}

	d3d9->Release();
	m_init = true;

	return true;
}

void JGE3D::run()
{
	if(!m_init)
	{
		return;
	}
	if(m_running)
	{
		return;
	}
	m_running = true;

	MSG msg;
	jgeZeroMem(&msg, sizeof(MSG));

	DWORD lastTime = timeGetTime();
	DWORD timeDeltaSum = 0;
	uint fps = 0;
	for(;;)
	{
		if(PeekMessage(&msg, null, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)	
			{
				break;
			}
			DispatchMessage(&msg);
			continue;
		}
		else
		{
			dword currTime;
			dword timeDelta;
			do 
			{
				currTime = timeGetTime();
				timeDelta = currTime - lastTime;
			} while (timeDelta < 1);
			
			if(jgeWin32KeyDown(VK_ESCAPE) && (m_wmEscapeKeyDownCallback == null || m_wmEscapeKeyDownCallback()))
			{
				jgeWin32DestroyWindow(m_hWnd);
				return;
			}

			m_lpd3dd->Clear(0, null, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0);
			m_lpd3dd->BeginScene();
			if(m_frameCallback != null)
			{
				m_frameCallback(timeDelta);
			}
			m_lpd3dd->EndScene();

			HRESULT presentH = m_lpd3dd->Present(null, null, null, null);
			// device lose
			if(presentH == D3DERR_DEVICELOST)
			{
				if(m_lpd3dd->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				{
					// release resource
					if(m_deviceLoseCallback != null)
					{
						m_deviceLoseCallback();
					}
						
					// reset device
					m_lpd3dd->Reset(&m_presentParams);

					// rebuild resource
					if(m_deviceLoseResetCallback != null)
					{
						if(!m_deviceLoseResetCallback())
						{
							break;
						}
					}
				}
			}

			timeDeltaSum += timeDelta;
			if(timeDeltaSum <= 1000)
			{
				++fps;
			}
			else
			{
				m_fps = fps;
				fps = 0;
				timeDeltaSum = 0;
			}
			lastTime = currTime;
		}
	}

	if(m_releaseCallback != null)
	{
		m_releaseCallback();
	}
}

void JGE3D::runManual()
{
	static RECT presentSrc = { 0, 0, m_viewPort.Width, m_viewPort.Height };
	static RECT presentDest = { m_viewPort.X, m_viewPort.Y, m_viewPort.X + m_viewPort.Width, m_viewPort.Y + m_viewPort.Height};

	static DWORD lastTime = timeGetTime();
	static DWORD timeDeltaSum = 0;
	static uint fps = 0;
	dword currTime;
	dword timeDelta;
	do 
	{
		currTime = timeGetTime();
		timeDelta = currTime - lastTime;
	} while (timeDelta < 1);

	m_lpd3dd->Clear(0, null, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0);
	m_lpd3dd->BeginScene();
	if(m_frameCallback != null)
	{
		m_frameCallback(timeDelta);
	}
	m_lpd3dd->EndScene();

	HRESULT presentH = m_lpd3dd->Present(&presentSrc, &presentDest, null, null);
	// device lose
	if(presentH == D3DERR_DEVICELOST)
	{
		if(m_lpd3dd->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			// release resource
			if(m_deviceLoseCallback != null)
			{
				m_deviceLoseCallback();
			}

			// reset device
			m_lpd3dd->Reset(&m_presentParams);

			// rebuild resource
			if(m_deviceLoseResetCallback != null)
			{
				if(!m_deviceLoseResetCallback())
				{
					return;
				}
			}
		}
	}

	timeDeltaSum += timeDelta;
	if(timeDeltaSum <= 1000)
	{
		++fps;
	}
	else
	{
		m_fps = fps;
		fps = 0;
		timeDeltaSum = 0;
	}
	lastTime = currTime;
}

void JGE3D::setFPS(uint value)
{
	m_fps = value;
	m_fpsTime = 1000 / m_fps;
}

bool JGE3D::setMessageCallback(uint msg, jge3d::WinProcCallback callback)
{
	clearMessageCallback(msg);
	jge3d::winProcCallbacks[msg] = callback;
	return true;
}

bool JGE3D::clearMessageCallback(uint msg)
{
	if(containsMesageCallback(msg))
	{
		jge3d::winProcCallbacks.erase(msg);
		return true;
	}
	else
	{
		return false;
	}
}

bool JGE3D::containsMesageCallback(uint msg) const
{
	return jge3d::winProcCallbacks.find(msg) != jge3d::winProcCallbacks.end();
}

jge3d::WinProcCallback JGE3D::getMessageCallback(uint msg) const
{
	if(containsMesageCallback(msg))
	{
		return jge3d::winProcCallbacks[msg];
	}
	else
	{
		return null;
	}
}

bool JGE3D::setRenderState(IDirect3DDevice9* lpd3dd, dword cullMode, bool lighting, bool zEnable, dword shadeMode, dword fillMode, bool alphaBlendEnable)
{
	if(lpd3dd == null)
	{
		return false;
	}
	else
	{
		if(FAILED(lpd3dd->SetRenderState(D3DRS_CULLMODE, cullMode)))
		{
			return false;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_LIGHTING, lighting)))
		{
			return false;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_ZENABLE, zEnable)))
		{
			return false;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_SHADEMODE, shadeMode)))
		{
			return false;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_FILLMODE, fillMode)))
		{
			return false;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlendEnable)))
		{
			return false;
		}

		return true;
	}
}

bool JGE3D::setProjectionPerspectiveTransform(IDirect3DDevice9* lpd3dd, uint windowWidth, uint windowHeight)
{
	if(lpd3dd == null)
	{
		return false;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixPerspectiveFovLH(&out, D3DX_PI * 0.5f, (float)windowWidth / (float)windowHeight, 1.0f, 1000.0f);
		if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &out)))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool JGE3D::setViewTransform(IDirect3DDevice9* lpd3dd, D3DXVECTOR3* lpeye, D3DXVECTOR3* lptarget, D3DXVECTOR3* lpup)
{
	if(lpd3dd == null || lpeye == null || lptarget == null || lpup == null)
	{
		return false;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixLookAtLH(&out, lpeye, lptarget, lpup);
		if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &out)))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool JGE3D::setViewTransform(IDirect3DDevice9* lpd3dd, float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ)
{
	if(lpd3dd == null)
	{
		return false;
	}
	else
	{
		D3DXMATRIX out;
		D3DXVECTOR3 eye(eyeX, eyeY, eyeZ);
		D3DXVECTOR3 target(targetX, targetY, targetZ);
		D3DXVECTOR3 up(upX, upY, upZ);
		D3DXMatrixLookAtLH(&out, &eye, &target, &up);
		if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &out)))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}