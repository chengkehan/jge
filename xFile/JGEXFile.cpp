#include <rmxftmpl.h>

#include "JGEXFile.h"

using namespace std;

JGEXFile::JGEXFile()
{

}

JGEXFile::~JGEXFile()
{

}

bool JGEXFile::parse(const char* lpXFileData)
{
	if(lpXFileData == null)
	{
		return false;
	}

	cleanUp();

	ID3DXFile* lpXFile = null;
	jgeDXVerifyIf(D3DXFileCreate(&lpXFile))
		jgeReleaseCom(lpXFile);
		return false;
	jgeDXVerifyEndIf

	jgeDXVerifyIf(lpXFile->RegisterTemplates(D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES))
		jgeReleaseCom(lpXFile);
		return false;
	jgeDXVerifyEndIf

	jgeDXVerifyIf(lpXFile->RegisterTemplates(lpXFileData, jgecslen(lpXFileData)))
		jgeReleaseCom(lpXFile);
		return false;
	jgeDXVerifyEndIf

	D3DXF_FILELOADMEMORY fileMemData;
	fileMemData.lpMemory = lpXFileData;
	fileMemData.dSize = jgecslen(lpXFileData);

	ID3DXFileEnumObject* lpEnum = null;
	jgeDXVerifyIf(lpXFile->CreateEnumObject(&fileMemData, D3DXF_FILELOAD_FROMMEMORY, &lpEnum))
		jgeReleaseCom(lpXFile);
		return false;
	jgeDXVerifyEndIf

	SIZE_T numChildren;
	jgeDXVerifyIf(lpEnum->GetChildren(&numChildren))
		jgeReleaseCom(lpXFile);
		jgeReleaseCom(lpEnum);
		return false;
	jgeDXVerifyEndIf

	for (SIZE_T childIndex = 0; childIndex < numChildren; ++childIndex)
	{
		ID3DXFileData* lpXFileData = null;
		jgeDXVerifyIf(lpEnum->GetChild(childIndex, &lpXFileData))
			break;
		jgeDXVerifyEndIf
		parseChildren(lpXFileData, false, null, null);
		jgeReleaseCom(lpXFileData);
	}

	jgeReleaseCom(lpXFile);
	jgeReleaseCom(lpEnum);

	return true;
}

void JGEXFile::parseChildren(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent)
{
	if(lpXFileData == null)
	{
		return;
	}

	GUID guid;
	jgeDXVerifyIf(lpXFileData->GetType(&guid))
		return;
	jgeDXVerifyEndIf

	void* lpData = parseChild(lpXFileData, isReference || lpXFileData->IsReference(), lpXFileDataParent, lpDataParent, &guid);

	SIZE_T numChlidren;
	jgeDXVerifyIf(lpXFileData->GetChildren(&numChlidren))
		return;
	jgeDXVerifyEndIf
	for (SIZE_T childIndex = 0; childIndex < numChlidren; ++childIndex)
	{
		ID3DXFileData* lpChildXFileData = null;
		jgeDXVerifyIf(lpXFileData->GetChild(childIndex, &lpChildXFileData))
			return;
		jgeDXVerifyEndIf
		parseChildren(lpChildXFileData, isReference || lpXFileData->IsReference() || lpChildXFileData->IsReference(), lpXFileData, lpData);
		jgeReleaseCom(lpChildXFileData);
	}
}