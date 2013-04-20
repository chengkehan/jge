#include "JGEXModel.h"

JGEXModel::JGEXModel(IDirect3DDevice9* lpd3dd)
{
	jgeAssert(lpd3dd);
	m_lpd3dd = lpd3dd;
	m_lpBone = null;
	m_lpSkinMesh = null;
	m_lpAnimation = null;
}

JGEXModel::~JGEXModel()
{
	cleanUp();	
}

void JGEXModel::draw()
{
	JGEXMeshContainer* lpSkinMesh = m_lpSkinMesh == null ? null : m_lpSkinMesh->getMeshContainerHeadNode();
	while(lpSkinMesh != null)
	{
		for (DWORD boneIndex = 0; boneIndex < lpSkinMesh->pSkinInfo->GetNumBones(); ++ boneIndex)
		{
			lpSkinMesh->lpBoneMatrices[boneIndex] = *lpSkinMesh->pSkinInfo->GetBoneOffsetMatrix(boneIndex);
			if(lpSkinMesh->lplpFrameMatrices[boneIndex] != null)
			{
				lpSkinMesh->lpBoneMatrices[boneIndex] *= *lpSkinMesh->lplpFrameMatrices[boneIndex];
			}
		}

		void* lpSrc, *lpDest;
		jgeDXVerifyIf(lpSkinMesh->lpSkinMesh->LockVertexBuffer(D3DLOCK_READONLY, &lpSrc))jgeDXVerifyEndIf
		jgeDXVerifyIf(lpSkinMesh->MeshData.pMesh->LockVertexBuffer(0, &lpDest))jgeDXVerifyEndIf
		jgeDXVerifyIf(lpSkinMesh->pSkinInfo->UpdateSkinnedMesh(lpSkinMesh->lpBoneMatrices, null, lpSrc, lpDest))jgeDXVerifyEndIf
		jgeDXVerifyIf(lpSkinMesh->lpSkinMesh->UnlockVertexBuffer())jgeDXVerifyEndIf
		jgeDXVerifyIf(lpSkinMesh->MeshData.pMesh->UnlockVertexBuffer())jgeDXVerifyEndIf

		for (DWORD materialIndex = 0; materialIndex < lpSkinMesh->NumMaterials; ++materialIndex)
		{
			jgeDXVerifyIf(m_lpd3dd->SetMaterial(&lpSkinMesh->pMaterials[materialIndex].MatD3D))jgeDXVerifyEndIf
			jgeDXVerifyIf(m_lpd3dd->SetTexture(0, lpSkinMesh->lplpTextures[materialIndex]))jgeDXVerifyEndIf
			jgeDXVerifyIf(lpSkinMesh->MeshData.pMesh->DrawSubset(materialIndex))jgeDXVerifyEndIf
		}

		lpSkinMesh = (JGEXMeshContainer*)lpSkinMesh->pNextMeshContainer;
	}
}

bool JGEXModel::parse(const char* lpXFileData)
{
	cleanUp();
	
	m_lpBone = new JGEXFileBone();
	if(!m_lpBone->parse(lpXFileData))
	{
		return false;
	}

	JGEXFileBoneTransform boneTransform(m_lpBone);
	if(!boneTransform.parse(lpXFileData))
	{
		return false;
	}

	m_lpSkinMesh = new JGEXFileSkinMesh(m_lpd3dd, m_lpBone);
	if(!m_lpSkinMesh->parse(lpXFileData))
	{
		return false;
	}

	m_lpAnimation = new JGEXFileAnimation(m_lpBone);
	if(!m_lpAnimation->parse(lpXFileData))
	{
		return false;
	}

	return true;
}

JGEXFileBone* JGEXModel::getBone()
{
	return m_lpBone;
}

JGEXFileSkinMesh* JGEXModel::getSkinMesh()
{
	return m_lpSkinMesh;
}

JGEXFileAnimation* JGEXModel::getAnimation()
{
	return m_lpAnimation;
}

void JGEXModel::cleanUp()
{
	jgeDelete(m_lpBone);
	jgeDelete(m_lpSkinMesh);
	jgeDelete(m_lpAnimation);
}