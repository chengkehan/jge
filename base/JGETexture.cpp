#include "JGETexture.h"

JGETexture::JGETexture(IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpImgInfo)
{
	m_lpTexture = lpTexture;
	m_lpImgInfo = null;
	if(lpImgInfo != null)
	{
		jgeNew(m_lpImgInfo, D3DXIMAGE_INFO);
		jgeMemCpy(lpImgInfo, m_lpImgInfo, sizeof(D3DXIMAGE_INFO));
	}
}

JGETexture::~JGETexture()
{
	jgeReleaseCom(m_lpTexture);
	jgeDelete(m_lpImgInfo);
}

IDirect3DTexture9* JGETexture::getTexture() const
{
	return m_lpTexture;
}

const D3DXIMAGE_INFO* JGETexture::getImageInfo() const
{
	return m_lpImgInfo;
}