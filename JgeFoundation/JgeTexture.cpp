#include "JgeStdafx.h"
#include "JgeTexture.h"
#include "JgeCommon.h"
#include "JgeMemory.h"
#include "JgeString.h"
#include "JgeIO.h"

jge::Texture::Texture():
	m_lpTexture(null), m_lpFileURL(null), m_lpBytesData(null)
{
	// Do nothing
}

jge::Texture::~Texture()
{
	release();
}

bool jge::Texture::load(const wchar_t* lpFileURL)
{
	if(lpFileURL == null)
	{
		return false;
	}

	return false;
}

bool jge::Texture::load(const char* lpBytesData)
{
	return false;
}

void jge::Texture::release()
{
	jgeReleaseCom(m_lpTexture);
	jgeFree(m_lpBytesData);
	jgewcsfree(m_lpFileURL);
}