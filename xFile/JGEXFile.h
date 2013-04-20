#ifndef __JC_XFILE_H__
#define __JC_XFILE_H__

#include <Windows.h>
#include <fstream>
#include <d3dx9xof.h>

#include "jgeUtil.h"

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

class JGEXFile
{
public:
	JGEXFile();
	~JGEXFile();

	bool parse(const char* lpXFileData);

protected:
	virtual void* parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid) = 0;
	virtual void cleanUp() = 0;

private:
	JGEXFile(const JGEXFile& value);

	void parseChildren(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent);
};

#endif