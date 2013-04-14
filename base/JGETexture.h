#ifndef __JGE_TEXTURE_H__
#define __JGE_TEXTURE_H__

#include <d3d9.h>
#include <d3dx9.h>
#include "JGEObject.h"
#include "jgeUtil.h"

class JGETexture : public JGEObject
{
public:
	JGETexture(IDirect3DDevice9* lpd3dd, const char* lpFile, D3DPOOL pool);
	~JGETexture();

	bool load();
	void deviceLose();
	bool deviceReset();

	inline IDirect3DTexture9* getTexture() const { return m_lpTexture; }
	inline const D3DXIMAGE_INFO* getImageInfo() const { return &m_imgInfo; };
	inline const D3DSURFACE_DESC* getSurfaceDesc() const { return &m_surfaceDesc; }

private:
	JGETexture();
	JGETexture(const JGETexture& value);

	IDirect3DDevice9* m_lpd3dd;
	IDirect3DTexture9* m_lpTexture;
	D3DXIMAGE_INFO m_imgInfo;
	D3DSURFACE_DESC m_surfaceDesc;
	char* m_lpMemData;
	uint m_memDataBytes;
	char* m_lpFile;
	D3DPOOL m_pool;

	bool loadFile();
	bool createTexture();
};

#endif