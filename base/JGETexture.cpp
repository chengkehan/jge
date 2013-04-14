#include "JGETexture.h"

JGETexture::JGETexture(IDirect3DDevice9* lpd3dd, const char* lpFile, D3DPOOL pool)
{
	jgeAssert(lpd3dd != null);
	jgeAssert(lpFile != null);
	m_lpd3dd = lpd3dd;
	m_pool = pool;
	m_lpFile = jgecsclone(lpFile);
	m_lpMemData = null;
	m_lpTexture = null;
}

JGETexture::~JGETexture()
{
	jgeReleaseCom(m_lpTexture);
	jgeDelete(m_lpMemData);
	jgecsfree(m_lpFile);
	m_lpd3dd = null;
}

bool JGETexture::load()
{
	return loadFile() && createTexture();
}

void JGETexture::deviceLose()
{
	if(m_pool != D3DPOOL_MANAGED)
	{
		jgeReleaseCom(m_lpTexture);
	}
}

bool JGETexture::deviceReset()
{
	if(m_pool != D3DPOOL_MANAGED)
	{
		return createTexture();
	}
	else
	{
		return true;
	}
}

bool JGETexture::loadFile()
{
	jgeDelete(m_lpMemData);

	if(!jgeReadFile(m_lpFile, null, &m_memDataBytes))
	{
		return false;
	}
	jgeMalloc(m_lpMemData, m_memDataBytes, char*);
	if(!jgeReadFile(m_lpFile, m_lpMemData, null))
	{
		return false;
	}
	
	return true;
}

bool JGETexture::createTexture()
{
	jgeReleaseCom(m_lpTexture);

	if(FAILED(D3DXCreateTextureFromFileInMemoryEx(
		m_lpd3dd, m_lpMemData, m_memDataBytes, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, m_pool, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, &m_imgInfo, NULL, &m_lpTexture
		)))
	{
		return false;
	}
	else
	{
		if(FAILED(m_lpTexture->GetLevelDesc(0, &m_surfaceDesc)))
		{
			return false;
		}
		
		return true;
	}
}