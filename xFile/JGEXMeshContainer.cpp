#include "JGEXMeshContainer.h"

JGEXMeshContainer::JGEXMeshContainer()
{
	Name = null;
	MeshData.pMesh = null;
	pMaterials = null;
	pEffects = null;
	NumMaterials = 0;
	pAdjacency = null;
	pSkinInfo = null;
	pNextMeshContainer = null;
	lplpTextures = null;
	lpSkinMesh = null;
	lplpFrameMatrices = null;
	lpBoneMatrices = null;
}

JGEXMeshContainer::~JGEXMeshContainer()
{
	if(lplpTextures != null && NumMaterials)
	{
		for (DWORD i = 0; i < NumMaterials; ++i)
		{
			if(lplpTextures[i] != null)
			{
				lplpTextures[i]->Release();
				lplpTextures[i] = null;
			}
		}
	}
	delete []lplpTextures; lplpTextures = null;

	NumMaterials = 0;

	delete []Name; Name = null;
	delete []pMaterials; pMaterials = null;
	delete pEffects; pEffects = null;

	delete []pAdjacency; pAdjacency = null;
	delete []lplpFrameMatrices; lplpFrameMatrices = null;
	delete []lpBoneMatrices; lpBoneMatrices = null;

	if(MeshData.pMesh != null)
	{
		MeshData.pMesh->Release();
		MeshData.pMesh = null;
	}
	if(pSkinInfo != null)
	{
		pSkinInfo->Release();
		pSkinInfo = null;
	}
	if(lpSkinMesh != null)
	{
		lpSkinMesh->Release();
		lpSkinMesh = null;
	}

	delete pNextMeshContainer; pNextMeshContainer = null;
}