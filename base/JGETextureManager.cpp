#include "JGETextureManager.h"

JGETextureManager::JGETextureManager()
{

}

JGETextureManager::~JGETextureManager()
{
	for (TextureMap::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		JGETexture* lpJGETexture = iter->second;
		jgeDelete(lpJGETexture);
	}
}

JGETexture* JGETextureManager::addTexture(int id, IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpInfo)
{
	if(lpTexture == null)
	{
		return null;
	}

	if(containsTexture(id))
	{
		return getTexture(id);
	}
	else
	{
		JGETexture* lpJGETexture = null;
		jgeNewArgs2(lpJGETexture, JGETexture, lpTexture, lpInfo);
		m_textureMap[id] = lpJGETexture;
		return lpJGETexture;
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

JGETexture* JGETextureManager::getTexture(int id)
{
	if(containsTexture(id))
	{
		return m_textureMap[id];
	}
	else
	{
		return null;
	}
}

JGETexture* JGETextureManager::loadFileTexture(int id, const char* lpPath, IDirect3DDevice9* lpd3dd)
{
	if(containsTexture(id))
	{
		return getTexture(id);
	}
	else
	{
		IDirect3DTexture9* lpTexture = NULL;
		D3DXIMAGE_INFO imgInfo;
		if(FAILED(D3DXCreateTextureFromFileExA(
			lpd3dd, lpPath, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &imgInfo, NULL, &lpTexture
		)))
		{
			return null;
		}
		else
		{
			JGETexture* lpJGETexture = null;
			jgeNewArgs2(lpJGETexture, JGETexture, lpTexture, &imgInfo);
			m_textureMap[id] = lpJGETexture;
			return lpJGETexture;
		}
	}
}

bool JGETextureManager::containsTexture(int id)
{
	return m_textureMap.find(id) != m_textureMap.end();
}