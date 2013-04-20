#ifndef __JC_XMODEL_H__
#define __JC_XMODEL_H__

#include <Windows.h>
#include <d3d9.h>

#include "JGEXFileBone.h"
#include "JGEXFileBoneTransform.h"
#include "JGEXFileSkinMesh.h"
#include "JGEXFileAnimation.h"
#include "jgeUtil.h"

class JGEXModel
{
public:
	JGEXModel(IDirect3DDevice9* lpd3dd);
	~JGEXModel();

	JGEXFileBone* getBone();
	JGEXFileSkinMesh* getSkinMesh();
	JGEXFileAnimation* getAnimation();
	void draw();
	bool parse(const char* lpXFileData);

private:
	JGEXModel();
	JGEXModel(const JGEXModel& value);

	JGEXFileBone* m_lpBone;
	JGEXFileSkinMesh* m_lpSkinMesh;
	JGEXFileAnimation* m_lpAnimation;
	IDirect3DDevice9* m_lpd3dd;

	void cleanUp();
};

#endif