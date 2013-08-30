#include "JgeStdafx.h"
#include "JgeMemory.h"

jge::Buffer::Buffer():
	m_lpBuffer(null), m_numBytes(0)
{
	// Do nothing
}

jge::Buffer::~Buffer()
{
	release();
}

bool jge::Buffer::setBuffer(const char* lpBuffer, uint numBytes)
{
	if(lpBuffer != null && numBytes > 0)
	{
		release();
		jgeMalloc(m_lpBuffer, numBytes, char*);
		jgeMemCpy(lpBuffer, m_lpBuffer, numBytes);
		m_numBytes = numBytes;
		return true;
	}
	else
	{
		return false;
	}
}

void jge::Buffer::release()
{
	jgeFree(m_lpBuffer);
	m_numBytes = 0;
}