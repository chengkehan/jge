#include "JgeStdafx.h"
#include "Jge3D.h"
#include "JgeMemory.h"
#include "JgeCommon.h"

jge::Jge3D::Jge3D():
	m_lpWindow(null), m_lpd3d(null), m_lpd3dd(null), m_deviceBehaviorFlags(0)
{

}

jge::Jge3D::~Jge3D()
{
	release();
}

bool jge::Jge3D::init(jge::Window* lpWindow)
{
	if(lpWindow == null || m_lpWindow != null|| m_lpd3d != null || m_lpd3dd != null)
	{
		return false;
	}
	m_lpWindow = lpWindow;

	m_lpd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_lpd3d == null)
	{
		return false;
	}

	D3DCAPS9 caps;
	jge3DIfFailed(m_lpd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps))
		release();
		return false;
	jge3DEndIf

	if(caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
	{
		release();
		return false;
	}
	if(caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		release();
		return false;
	}

	m_deviceBehaviorFlags = caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	m_presentParams.BackBufferWidth = m_lpWindow->getWindowWidth();
	m_presentParams.BackBufferHeight = m_lpWindow->getWindowHeight();
	m_presentParams.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_presentParams.BackBufferCount = 1;
	m_presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_presentParams.MultiSampleQuality = 0;
	m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_presentParams.hDeviceWindow = m_lpWindow->getHWnd();
	m_presentParams.Windowed = m_lpWindow->getWindowd();
	m_presentParams.EnableAutoDepthStencil = true;
	m_presentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_presentParams.Flags = 0;
	m_presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	jge3DIfFailed(m_lpd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_lpWindow->getHWnd(), m_deviceBehaviorFlags, &m_presentParams, &m_lpd3dd))
		release();
		return false;
	jge3DEndIf

	return true;
}

bool jge::Jge3D::resetWindow()
{
	if(m_lpWindow == null || m_lpd3d == null || m_lpd3dd == null)
	{
		return false;
	}
	
	if(m_lpWindow->getWindowWidth() == m_presentParams.BackBufferWidth && m_lpWindow->getWindowHeight() == m_presentParams.BackBufferHeight && 
		m_lpWindow->getHWnd() == m_presentParams.hDeviceWindow && m_lpWindow->getWindowd() == jgeToBoolean(m_presentParams.Windowed))
	{
		return false;
	}

	jgeReleaseCom(m_lpd3dd);
	m_presentParams.BackBufferWidth = m_lpWindow->getWindowWidth();
	m_presentParams.BackBufferHeight = m_lpWindow->getWindowHeight();
	m_presentParams.hDeviceWindow = m_lpWindow->getHWnd();
	m_presentParams.Windowed = m_lpWindow->getWindowd();
	jge3DIfFailed(m_lpd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_lpWindow->getHWnd(), m_deviceBehaviorFlags, &m_presentParams, &m_lpd3dd))
		release();
		return false;
	jge3DEndIf

	return true;
}

bool jge::Jge3D::resetDevice()
{
	if(m_lpWindow == null || m_lpd3d == null || m_lpd3dd == null)
	{
		return false;
	}

	jge3DIfSuccess(m_lpd3dd->Reset(&m_presentParams))
		return true;
	jge3DEndIf
	return false;
}

void jge::Jge3D::release()
{
	m_deviceBehaviorFlags = 0;
	m_lpWindow = null;
	jgeReleaseCom(m_lpd3d);
	jgeReleaseCom(m_lpd3dd);
}