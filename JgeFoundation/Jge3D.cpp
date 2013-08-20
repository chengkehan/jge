#include "JgeStdafx.h"
#include "Jge3D.h"
#include "JgeMemory.h"

jge::Jge3D::Jge3D():
	m_lpWindow(null), m_lpd3d(null), m_lpd3dd(null)
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

	return true;
}

void jge::Jge3D::release()
{
	m_lpWindow = null;
	jgeReleaseCom(m_lpd3d);
	jgeReleaseCom(m_lpd3dd);
}