#ifndef __JGE_RENDER_H__
#define __JGE_RENDER_H__

#include <d3d9.h>
#include "JGEDisplayObject.h"

class JGERender
{
JGE_SINGLETON_DECLARE(JGERender)

public:
	bool init(IDirect3DDevice9* lpd3dd, int bufferDisplayObjectAmount = 4000);
	void beginScene();
	bool renderDisplayObject(JGEDisplayObject* lpDisplayObject);
	void endScene();

private:
	JGERender();
	~JGERender();

	IDirect3DDevice9* m_lpd3dd;
	IDirect3DVertexBuffer9* m_lpVbBuffer;
	JGEDisplayObject::Vertex* m_lpVbBufferData;
	IDirect3DIndexBuffer9* m_lpIbBuffer;
	word* m_lpIbBufferData;
	int m_bufferDisplayObjectAmount;
	int m_displayObjectCount;
	IDirect3DTexture9* m_lpTextureCurrent;
	bool m_alphaEnbaledCurrent;
	bool m_vbibLocked;

	void renderBuffer(bool relock);
	void lockVbIb();
	void unlockVbIb();
};

#endif