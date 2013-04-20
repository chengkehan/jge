#ifndef __JC_XFILE_ANIMATION_H__
#define __JC_XFILE_ANIMATION_H__

#include <Windows.h>
#include <rmxfguid.h>
#include <d3dx9.h>

#include "JGEXFile.h"
#include "JGEXFileBone.h"
#include "JGEAnimationSet.h"
#include "JGEAnimation.h"
#include "JGEXFrame.h"
#include "jgeUtil.h"

class JGEXFileAnimation : public JGEXFile
{
public:
	JGEXFileAnimation(JGEXFileBone* lpXFileBone);
	~JGEXFileAnimation();

	bool play(const char* lpAnimationSetName, DWORD time);

protected:
	virtual void* parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid);
	virtual void cleanUp();

private:
	JGEXFileAnimation();
	JGEXFileAnimation(const JGEXFileAnimation& value);

	JGEAnimationSet* m_lpAnimationSetRoot;
	JGEXFileBone* m_lpXFileBone;
};

#endif