#include "JGETextureManager.h"

JGE_SINGLETON_IMPLEMENTS(JGETextureManager)

JGETextureManager::JGETextureManager()
{

}

JGETextureManager::~JGETextureManager()
{
	for (TextureMap::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		jgeDelete(iter->second);
	}
}

bool JGETextureManager::removeTexture(int id)
{
	TextureMap::iterator iter = m_textureMap.find(id);
	if(iter == m_textureMap.end())
	{
		return false;
	}
	else
	{
		JGETexture* lpJGETexture = iter->second;
		jgeDelete(lpJGETexture);
		m_textureMap.erase(iter);
		return true;
	}
}

JGETexture* JGETextureManager::getTexture(int id) const
{
	TextureMap::const_iterator iter = m_textureMap.find(id);
	if(iter == m_textureMap.end())
	{
		return null;
	}
	else
	{
		return iter->second;
	}
}

JGETexture* JGETextureManager::loadTextureFromFile(int id, const char* lpPath, IDirect3DDevice9* lpd3dd, D3DPOOL pool)
{
	if(containsTexture(id))
	{
		return getTexture(id);
	}
	else
	{
		JGETexture* lpJGETexture = null;
		jgeNewArgs3(lpJGETexture, JGETexture, lpd3dd, lpPath, pool);
		if(lpJGETexture->load())
		{
			m_textureMap[id] = lpJGETexture;
			return lpJGETexture;
		}
		else
		{
			jgeDelete(lpJGETexture);
			return null;
		}
	}
}

bool JGETextureManager::containsTexture(int id) const
{
	return m_textureMap.find(id) != m_textureMap.end();
}

void JGETextureManager::deviceLose()
{
	for (TextureMap::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		iter->second->deviceLose();
	}
}

bool JGETextureManager::deviceReset()
{
	for (TextureMap::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		if(!iter->second->deviceReset())
		{
			return false;
		}
	}

	return true;
}