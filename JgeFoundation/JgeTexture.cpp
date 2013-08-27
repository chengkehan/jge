#include "JgeStdafx.h"
#include "JgeTexture.h"
#include "JgeCommon.h"
#include "JgeMemory.h"
#include "JgeString.h"
#include "JgeIO.h"

jge::Texture::Texture():
	m_lpTexture(null), m_lpFileURL(null), m_lpBytesData(null), m_numBytes(0)
{
	// Do nothing
}

jge::Texture::~Texture()
{
	release();
}

bool jge::Texture::load(const wchar_t* lpFileURL)
{
	uint numBytes = 0;
	if(!jge::File::read(lpFileURL, null, &numBytes, true))
	{
		return false;
	}
	char* lpBytesData = null;
	jgeMalloc(lpBytesData, numBytes, char*);
	if(!jge::File::read(lpFileURL, lpBytesData, null, true))
	{
		jgeFree(lpBytesData);
		return false;
	}

	release();
	m_lpBytesData = lpBytesData;
	m_numBytes = numBytes;
	m_lpFileURL = jgewcsclone(lpFileURL);

	return true;
}

bool jge::Texture::load(const char* lpBytesData, uint numBytes)
{
	if(lpBytesData != null && numBytes > 0)
	{
		release();
		jgeMalloc(m_lpBytesData, numBytes, char*);
		jgeMemCpy(lpBytesData, m_lpBytesData, numBytes);
		m_numBytes = numBytes;
		return true;
	}
	else
	{
		return false;
	}
}

void jge::Texture::release()
{
	jgeReleaseCom(m_lpTexture);
	jgeFree(m_lpBytesData);
	jgewcsfree(m_lpFileURL);
	m_numBytes = 0;
}