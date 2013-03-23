#include "JGETexture.h"

JGETexture::JGETexture(IDirect3DTexture9* lpTexture, const D3DXIMAGE_INFO* lpImgInfo, const D3DSURFACE_DESC* lpSurfaceDesc)
{
	jgeAssert(lpImgInfo != null);
	jgeAssert(lpSurfaceDesc != null);

	m_lpTexture = lpTexture;
	m_imgInfo = *lpImgInfo;
	m_surfaceDesc = *lpSurfaceDesc;
}

JGETexture::~JGETexture()
{
	jgeReleaseCom(m_lpTexture);
}