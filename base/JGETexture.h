#ifndef __JGE_TEXTURE_H__
#define __JGE_TEXTURE_H__

#include <d3d9.h>
#include <d3dx9.h>
#include "JGEObject.h"
#include "jgeUtil.h"

class JGETexture : public JGEObject
{
public:
	JGETexture(IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpImgInfo = null);
	~JGETexture();

	inline IDirect3DTexture9* getTexture() const { return m_lpTexture; }
	inline const D3DXIMAGE_INFO* getImageInfo() const { return m_lpImgInfo; };

private:
	JGETexture();
	JGETexture(const JGETexture& value);

	IDirect3DTexture9* m_lpTexture;
	D3DXIMAGE_INFO* m_lpImgInfo;
};

#endif