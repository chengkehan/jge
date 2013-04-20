#include "JGEXFileBone.h"

JGEXFileBone::JGEXFileBone():m_boneRoot(null)
{

}

JGEXFileBone::~JGEXFileBone()
{
	jgeDelete(m_boneRoot);
}

JGEXFrame* JGEXFileBone::getRootBone()
{
	return m_boneRoot;
}

JGEXFrame* JGEXFileBone::findBone(const char* lpBoneName)
{
	return fineBoneRecursion(lpBoneName, m_boneRoot);
}

bool JGEXFileBone::updateHierarchy(JGEXFrame* lpBone, D3DXMATRIX* lpTransform)
{
	if(lpBone == null)
	{
		return false;
	}

	D3DXMATRIX matrixIdentity;
	if(lpTransform == null)
	{
		D3DXMatrixIdentity(&matrixIdentity);
		lpTransform = &matrixIdentity;
	}

	lpBone->matrixCombine = lpBone->TransformationMatrix * (*lpTransform);
	updateHierarchy((JGEXFrame*)lpBone->pFrameSibling, lpTransform);
	updateHierarchy((JGEXFrame*)lpBone->pFrameFirstChild, &lpBone->matrixCombine);

	return true;
}

bool JGEXFileBone::resetHierarchy(JGEXFrame* lpBone)
{
	if(lpBone == null)
	{
		return false;
	}

	lpBone->TransformationMatrix = lpBone->matrixOriginal;
	resetHierarchy((JGEXFrame*)lpBone->pFrameSibling);
	resetHierarchy((JGEXFrame*)lpBone->pFrameFirstChild);

	return true;
}

void* JGEXFileBone::parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid)
{	
	if(*lpGuid == TID_D3DRMFrame && !isReference)
	{
		JGEXFrame* lpBone = new JGEXFrame();

		char* lpBoneName = null;
		SIZE_T boneNameNumChars = 0;
		jgeDXVerifyIf(lpXFileData->GetName(null, &boneNameNumChars))
			return null;
		jgeDXVerifyEndIf
		lpBoneName = new char[boneNameNumChars];
		lpBone->Name = lpBoneName;
		jgeDXVerifyIf(lpXFileData->GetName(lpBoneName, &boneNameNumChars))
			jgeDelete(lpBone);
			return null;
		jgeDXVerifyEndIf

		if(lpXFileDataParent == null)
		{
			lpBone->pFrameSibling = m_boneRoot;
			m_boneRoot = lpBone;
		}
		else
		{
			jgeAssert(lpDataParent != null);
			JGEXFrame* lpBoneParent = (JGEXFrame*)lpDataParent;
			lpBone->pFrameSibling = lpBoneParent->pFrameFirstChild;
			lpBoneParent->pFrameFirstChild = lpBone;

		}

		return lpBone;
	}
	else
	{
		return null;
	}
}

void JGEXFileBone::cleanUp()
{
	jgeDelete(m_boneRoot);
}

JGEXFrame* JGEXFileBone::fineBoneRecursion(const char* lpBoneName, JGEXFrame* lpBone)
{
	if(lpBone == null)
	{
		return null;
	}

	if(jgecsequ(lpBoneName, lpBone->Name))
	{
		return lpBone;
	}

	JGEXFrame* lpBoneResult = fineBoneRecursion(lpBoneName, (JGEXFrame*)lpBone->pFrameSibling);
	if(lpBoneResult != null)
	{
		return lpBoneResult;
	}

	lpBoneResult = fineBoneRecursion(lpBoneName, (JGEXFrame*)lpBone->pFrameFirstChild);
	if(lpBoneResult != null)
	{
		return lpBoneResult;
	}

	return null;
}