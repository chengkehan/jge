#include "JGEXFileSkinMesh.h"

JGEXFileSkinMesh::JGEXFileSkinMesh(IDirect3DDevice9* lpd3dd, JGEXFileBone* lpXFileBone):m_lpMeshContainerHeadNode(null),m_lpd3dd(lpd3dd), m_lpXFileBone(lpXFileBone)
{
	jgeAssert(lpd3dd != null);
	jgeAssert(lpXFileBone != null);
}

JGEXFileSkinMesh::~JGEXFileSkinMesh()
{

}

JGEXMeshContainer* JGEXFileSkinMesh::getMeshContainerHeadNode()
{
	return m_lpMeshContainerHeadNode;
}

void* JGEXFileSkinMesh::parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid)
{
	if(*lpGuid == TID_D3DRMMesh && !isReference)
	{
		ID3DXBuffer* lpMaterials = null;
		DWORD numMaterials = 0;
		ID3DXSkinInfo* lpSkinInfo = null;
		ID3DXMesh* lpMesh = null;
		ID3DXBuffer* lpAdjacency = null;
		jgeDXVerifyIf(D3DXLoadSkinMeshFromXof(lpXFileData, D3DXMESH_SYSTEMMEM, m_lpd3dd, &lpAdjacency, &lpMaterials, null, &numMaterials, &lpSkinInfo, &lpMesh))
			return null;
		jgeDXVerifyEndIf
		jgeAssert(lpMesh != null);
		jgeAssert(lpSkinInfo != null);
		DWORD numBones = lpSkinInfo->GetNumBones();
		jgeAssert(numBones > 0);
		jgeReleaseCom(lpAdjacency);

		JGEXMeshContainer* lpMeshContainer = new JGEXMeshContainer();
		lpMeshContainer->pNextMeshContainer = m_lpMeshContainerHeadNode;
		m_lpMeshContainerHeadNode = lpMeshContainer;

		lpMeshContainer->MeshData.pMesh = lpMesh;
		lpMeshContainer->pSkinInfo = lpSkinInfo;
		lpMeshContainer->NumMaterials = numMaterials;
		lpMeshContainer->pMaterials = new D3DXMATERIAL[numMaterials];
		lpMeshContainer->lplpTextures = new IDirect3DTexture9*[numMaterials];
		
		D3DXMATERIAL* lpMaterialMemory = (D3DXMATERIAL*)lpMaterials->GetBufferPointer();
		for (DWORD materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			lpMeshContainer->pMaterials[materialIndex].MatD3D = lpMaterialMemory[materialIndex].MatD3D;
			lpMeshContainer->pMaterials[materialIndex].pTextureFilename = null;
			lpMeshContainer->lplpTextures[materialIndex] = null;
			if(lpMaterialMemory[materialIndex].pTextureFilename != null)
			{
				jgeDXVerifyIf(D3DXCreateTextureFromFileA(m_lpd3dd, lpMaterialMemory[materialIndex].pTextureFilename, &lpMeshContainer->lplpTextures[materialIndex]))
					// Do nothing
				jgeDXVerifyEndIf
			}
		}
		jgeReleaseCom(lpMaterials);

		jgeDXVerifyIf(lpMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, lpMeshContainer->MeshData.pMesh->GetFVF(), m_lpd3dd, &lpMeshContainer->lpSkinMesh))
			return null;
		jgeDXVerifyEndIf

		lpMeshContainer->lplpFrameMatrices = new D3DXMATRIX*[numBones];
		lpMeshContainer->lpBoneMatrices = new D3DXMATRIX[numBones];
		for (DWORD boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			const char* lpBoneName = lpSkinInfo->GetBoneName(boneIndex);
			JGEXFrame* lpBone = m_lpXFileBone->findBone(lpBoneName);
			jgeAssert(lpBone != null);
			lpMeshContainer->lplpFrameMatrices[boneIndex] = &lpBone->matrixCombine;
		}

		return null;
	}
	else
	{
		return null;
	}
}

void JGEXFileSkinMesh::cleanUp()
{
	jgeDelete(m_lpMeshContainerHeadNode);
}