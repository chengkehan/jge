#ifndef __JGE_RENDER_H__
#define __JGE_RENDER_H__

#include <d3d9.h>
#include "JGEDisplayObject.h"

class JGERender
{
JGE_SINGLETON_DECLARE(JGERender)

public:
	bool init(IDirect3DDevice9* lpd3dd, uint bufferDisplayObjectAmount = 500);
	void beginScene();
	bool renderDisplayObject(JGEDisplayObject* lpDisplayObject);
	void endScene();
	void deviceLose();
	bool deviceReset();

private:
	JGERender();
	~JGERender();

	IDirect3DDevice9* m_lpd3dd;
	IDirect3DVertexBuffer9* m_lpVbBuffer;
	JGEDisplayObject::Vertex* m_lpVbBufferData;
	IDirect3DIndexBuffer9* m_lpIbBuffer;
	word* m_lpIbBufferData;
	uint m_bufferDisplayObjectAmount;
	uint m_displayObjectCount;
	IDirect3DTexture9* m_lpTextureCurrent;
	bool m_alphaEnbaledCurrent;
	bool m_vbibLocked;

	void renderBuffer(bool relock);
	void lockVbIb();
	void unlockVbIb();
};

#endif