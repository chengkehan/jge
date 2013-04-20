#ifndef __JC_XFILE_SKINMESH_H__
#define __JC_XFILE_SKINMESH_H__

#include <Windows.h>
#include <rmxfguid.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "JGEXFile.h"
#include "JGEXMeshContainer.h"
#include "JGEXFileBone.h"
#include "JGEXFrame.h"
#include "jgeUtil.h"

class JGEXFileSkinMesh : public JGEXFile
{
public:
	JGEXFileSkinMesh(IDirect3DDevice9* lpd3dd, JGEXFileBone* lpXFileBone);
	~JGEXFileSkinMesh();

	JGEXMeshContainer* getMeshContainerHeadNode();

protected:
	virtual void* parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid);
	virtual void cleanUp();

private:
	JGEXFileSkinMesh();
	JGEXFileSkinMesh(const JGEXFileSkinMesh& value);

	JGEXMeshContainer* m_lpMeshContainerHeadNode;
	IDirect3DDevice9* m_lpd3dd;
	JGEXFileBone* m_lpXFileBone;
};

#endif