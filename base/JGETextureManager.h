#ifndef __JGE_TEXTUREMANAGER_H__
#define __JGE_TEXTUREMANAGER_H__

#include <map>
#include "JGETexture.h"
#include "jgeUtil.h"

class JGETextureManager
{
JGE_SINGLETON_DECLARE(JGETextureManager)

public:
	bool removeTexture(int id);
	JGETexture* getTexture(int id) const;
	JGETexture* loadTextureFromFile(int id, const char* lpPath, IDirect3DDevice9* lpd3dd, D3DPOOL pool = D3DPOOL_MANAGED);
	bool containsTexture(int id) const;

	void deviceLose();
	bool deviceReset();

private:
	JGETextureManager();
	~JGETextureManager();

	// key id, value lpTexture
	typedef std::map<int, JGETexture*> TextureMap;

	TextureMap m_textureMap;
};

#endif