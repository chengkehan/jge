#include "JGE3D.h"

JGE_SINGLETON_IMPLEMENTS(JGE3D)

jge3d::WinProcCallbackMap jge3d::winProcCallbacks;

LRESULT CALLBACK jge3d::winProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam)
{
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
	m_windowX = 0; m_windowY = 0; m_windowWidth = 0; m_windowHeight = 0;
	m_windowd = false;
	m_init = false;
	m_running = false;
	setupCallback = null;
	releaseCallback = null;
	frameCallback = null;
}

JGE3D::~JGE3D()
{
	if(releaseCallback != null)
	{
		releaseCallback();
	}
	jgeReleaseCom(m_lpd3dd);
}

bool JGE3D::init(HINSTANCE hInstance, int windowX, int windowY, uint windowWidth, uint windowHeight, 
	bool windowed, D3DDEVTYPE deviceType, uint maxTextureBlendStages, uint fps)
{
	if(m_init)
	{
		return true;
	}

	m_hInstance = hInstance;
	m_windowX = windowX;
	m_windowY = windowY;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	setFPS(fps);

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

	HWND hwnd = CreateWindow(L"jcd3dApp", L"jcd3dApp", WS_EX_TOPMOST, windowX, windowY, windowWidth, windowHeight, null, null, hInstance, null);
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
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if(caps.MaxTextureBlendStages < maxTextureBlendStages)
	{
		jgeMessageBoxError("MaxTextureBlendStages Failed");
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = windowWidth;
	d3dpp.BackBufferHeight = windowHeight;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, &m_lpd3dd);
	if(FAILED(hr))
	{
		d3d9->Release();
		jgeMessageBoxError("CreateDevice Failed");
		return false;
	}

	if(setupCallback != null)
	{
		if(!setupCallback())
		{
			d3d9->Release();
			if(releaseCallback != null)
			{
				releaseCallback();
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

	dword lastTime = timeGetTime();
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, null, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dword currTime = timeGetTime();
			if(currTime - lastTime >= m_fpsTime)
			{
				dword timeDelta = currTime - lastTime;
				m_lpd3dd->Clear(0, null, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
				m_lpd3dd->BeginScene();
				if(frameCallback != null)
				{
					frameCallback(currTime - lastTime);
				}
				m_lpd3dd->EndScene();
				m_lpd3dd->Present(null, null, null, null);
				lastTime = currTime;
			}
		}
	}
}

HWND JGE3D::getHWnd() const
{
	return m_hWnd;
}

HINSTANCE JGE3D::getHInstance() const
{
	return m_hInstance;
}

IDirect3DDevice9* JGE3D::getDirect3DDevice() const
{
	return m_lpd3dd;
}

int JGE3D::getWindowX() const
{
	return m_windowX;
}

int JGE3D::getWindowY() const
{
	return m_windowY;
}

uint JGE3D::getWindowWidth() const
{
	return m_windowWidth;
}

uint JGE3D::getWindowHeight() const
{
	return m_windowHeight;
}

uint JGE3D::getFPS() const
{
	return m_fps;
}

void JGE3D::setFPS(uint value)
{
	m_fps = value;
	m_fpsTime = 1000 / m_fps;
}

bool JGE3D::getWindowd() const
{
	return m_windowd;
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
	if(lpd3dd == NULL)
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
	if(lpd3dd == NULL || lpeye == NULL || lptarget == NULL || lpup == NULL)
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
	if(lpd3dd == NULL)
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