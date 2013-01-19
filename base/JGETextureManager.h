#ifndef __JGE_TEXTUREMANAGER_H__
#define __JGE_TEXTUREMANAGER_H__

#include <map>
#include "JGETexture.h"

class JGETextureManager
{
public:
	JGETextureManager();
	~JGETextureManager();

	JGETexture* addTexture(int id, IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpInfo = null);
	bool removeTexture(int id);
	JGETexture* getTexture(int id) const;
	JGETexture* loadFileTexture(int id, const char* lpPath, IDirect3DDevice9* lpd3dd);
	bool containsTexture(int id) const;

private:
	JGETextureManager(const JGETextureManager& value);

	// key id, value lpTexture
	typedef std::map<int, JGETexture*> TextureMap;

	TextureMap m_textureMap;
};

#endif