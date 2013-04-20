#ifndef __JC_XFILE_BONE_H__
#define __JC_XFILE_BONE_H__

#include <Windows.h>
#include <rmxfguid.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "JGEXFile.h"
#include "JGEXFrame.h"

class JGEXFileBone : public JGEXFile
{
public:
	JGEXFileBone();
	~JGEXFileBone();

	JGEXFrame* getRootBone();
	JGEXFrame* findBone(const char* lpBoneName);
	static bool updateHierarchy(JGEXFrame* lpBone, D3DXMATRIX* lpTransform = null);
	static bool resetHierarchy(JGEXFrame* lpBone);

protected:
	virtual void* parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid);
	virtual void cleanUp();

private:
	JGEXFrame* m_boneRoot;
	JGEXFileBone(const JGEXFileBone& value);
	JGEXFrame* fineBoneRecursion(const char* lpBoneName, JGEXFrame* lpBone);
};

#endif