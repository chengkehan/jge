#include "JgeStdafx.h"
#include "JgeIO.h"
#include "JgeMemory.h"
#include "JgeString.h"

jge::File::File():
	m_lpPath(null)
{
	// Do nothing
}

jge::File::~File()
{
	release();
}

bool jge::File::open(const wchar_t* lpPath)
{
	if(lpPath == null)
	{
		return false;
	}

	release();
	m_lpPath = jgewcsclone(lpPath);
	return true;
}

bool jge::File::exist()
{
	std::ifstream reader;
	reader.open(m_lpPath);
	bool r = reader.good();
	reader.close();
	return r;
}

bool jge::File::read(jge::Buffer* lpBuffer, bool binary)
{
	if(lpBuffer == null || m_lpPath == null)
	{
		return false;
	}

	uint numBytes;
	if(!readInvoke(null, &numBytes, binary))
	{
		return false;
	}
	char* lpBufferData = null;
	jgeMalloc(lpBufferData, numBytes, char*);
	if(!readInvoke(lpBufferData, null, binary))
	{
		jgeFree(lpBufferData);
		return false;
	}
	lpBuffer->setBuffer(lpBufferData, numBytes);
	jgeFree(lpBufferData);
	return true;
}

void jge::File::release()
{
	jgewcsfree(m_lpPath);
}

bool jge::File::exist(const wchar_t* lpPath)
{
	jge::File file;
	file.open(lpPath);
	return file.exist();
}

bool jge::File::readInvoke(char* lpFileData, uint* numBytes, bool binary)
{
	if(m_lpPath == null)
	{
		return false;
	}

	std::ifstream reader;
	reader.open(m_lpPath, std::ios_base::in | (binary ? std::ios_base::binary : 0));
	bool r = false;
	if(reader.good())
	{
		reader.seekg(0, std::ios_base::end);
		uint bytes = (uint)reader.tellg();
		reader.seekg(0, std::ios_base::beg);
		if(lpFileData != null)
		{
			*numBytes = bytes;
		}
		if(lpFileData != null)
		{
			jgeZeroMem(lpFileData, bytes);
			reader.read(lpFileData, bytes);
		}
		r = true;
	}
	reader.close();
	return r;
}