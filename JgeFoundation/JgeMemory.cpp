#include "JgeStdafx.h"
#include "JgeMemory.h"

jge::Buffer::Buffer(const char* lpBuffer, uint numBytes):
	m_lpBuffer(0), m_numBytes(0)
{
	jgeAssert(lpBuffer != null);
	jgeMalloc(m_lpBuffer, numBytes, char*);
	jgeMemCpy(lpBuffer, m_lpBuffer, numBytes);
	m_numBytes = numBytes;
}

jge::Buffer::~Buffer()
{
	jgeFree(m_lpBuffer);
	m_numBytes = 0;
}