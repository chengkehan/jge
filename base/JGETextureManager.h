#ifndef __JGE_TEXTUREMANAGER_H__
#define __JGE_TEXTUREMANAGER_H__

#include <map>
#include "JGETexture.h"
#include "jgeUtil.h"

class JGETextureManager
{
JGE_SINGLETON_DECLARE(JGETextureManager)

public:
	JGETexture* addTexture(int id, IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpInfo = null);
	bool removeTexture(int id);
	JGETexture* getTexture(int id) const;
	JGETexture* loadFileTexture(int id, const char* lpPath, IDirect3DDevice9* lpd3dd);
	bool containsTexture(int id) const;

private:
	JGETextureManager();
	~JGETextureManager();

	// key id, value lpTexture
	typedef std::map<int, JGETexture*> TextureMap;

	TextureMap m_textureMap;
};

#endif