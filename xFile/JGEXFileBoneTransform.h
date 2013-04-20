#ifndef __JC_XFILE_BONE_TRANSFORM_H__
#define __JC_XFILE_BONE_TRANSFORM_H__

#include <Windows.h>
#include <rmxfguid.h>

#include "JGEXFileBone.h"
#include "JGEXFrame.h"
#include "JGEXFile.h"

class JGEXFileBoneTransform : public JGEXFile
{
public:
	JGEXFileBoneTransform(JGEXFileBone* lpXFileBone);
	~JGEXFileBoneTransform();

protected:
	virtual void* parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid);
	virtual void cleanUp();

private:
	JGEXFileBoneTransform();
	JGEXFileBoneTransform(const JGEXFileBoneTransform* value);

	JGEXFileBone* m_lpXFileBone;
};

#endif