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

JGETexture* JGETextureManager::addTexture(int id, IDirect3DTexture9* lpTexture, const D3DXIMAGE_INFO* lpInfo, const D3DSURFACE_DESC* lpSurfaceDesc)
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
		jgeNewArgs3(lpJGETexture, JGETexture, lpTexture, lpInfo, lpSurfaceDesc);
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

JGETexture* JGETextureManager::loadTextureFromFile(int id, const char* lpPath, IDirect3DDevice9* lpd3dd)
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
			lpd3dd, lpPath, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, &imgInfo, NULL, &lpTexture
		)))
		{
			return null;
		}
		else
		{
			D3DSURFACE_DESC desc;
			if(FAILED(lpTexture->GetLevelDesc(0, &desc)))
			{
				jgeReleaseCom(lpTexture);
				return null;
			}
			JGETexture* lpJGETexture = null;
			jgeNewArgs3(lpJGETexture, JGETexture, lpTexture, &imgInfo, &desc);
			m_textureMap[id] = lpJGETexture;
			return lpJGETexture;
		}
	}
}

bool JGETextureManager::containsTexture(int id) const
{
	return m_textureMap.find(id) != m_textureMap.end();
}