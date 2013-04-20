#include "JGEXFileBoneTransform.h"

JGEXFileBoneTransform::JGEXFileBoneTransform(JGEXFileBone* lpXFileBone):m_lpXFileBone(lpXFileBone)
{
	jgeAssert(lpXFileBone != null);
}

JGEXFileBoneTransform::~JGEXFileBoneTransform()
{

}

void* JGEXFileBoneTransform::parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid)
{
	if(*lpGuid == TID_D3DRMFrameTransformMatrix && !isReference)
	{
		jgeAssert(lpXFileDataParent != null);

		GUID guidParent;
		jgeDXVerifyIf(lpXFileDataParent->GetType(&guidParent))
			return null;
		jgeDXVerifyEndIf
		jgeAssert(guidParent == TID_D3DRMFrame);

		SIZE_T boneNameNumChars;
		char* lpBoneName;
		jgeDXVerifyIf(lpXFileDataParent->GetName(null, &boneNameNumChars))
			return null;
		jgeDXVerifyEndIf
		lpBoneName = new char[boneNameNumChars];
		jgeDXVerifyIf(lpXFileDataParent->GetName(lpBoneName, &boneNameNumChars))
			jgeDeleteArray(lpBoneName);
			return null;
		jgeDXVerifyEndIf

		JGEXFrame* lpBone = m_lpXFileBone->findBone(lpBoneName);
		jgeDeleteArray(lpBoneName);
		jgeAssert(lpBone != null);

		SIZE_T dataSize;
		const void* lpData;
		jgeDXVerifyIf(lpXFileData->Lock(&dataSize, &lpData))
			return null;
		jgeDXVerifyEndIf

		lpBone->TransformationMatrix = *(D3DXMATRIX*)lpData;
		lpBone->matrixOriginal = lpBone->TransformationMatrix;

		jgeDXVerifyIf(lpXFileData->Unlock())
			return null;
		jgeDXVerifyEndIf

		return null;
	}
	else
	{
		return null;
	}
}

void JGEXFileBoneTransform::cleanUp()
{
	// Do nothing
}