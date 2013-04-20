#include "JGEXFileAnimation.h"

JGEXFileAnimation::JGEXFileAnimation(JGEXFileBone* lpXFileBone)
{
	jgeAssert(lpXFileBone != null);
	m_lpAnimationSetRoot = null;
	m_lpXFileBone = lpXFileBone;
}

JGEXFileAnimation::~JGEXFileAnimation()
{
	cleanUp();
}

bool JGEXFileAnimation::play(const char* lpAnimationSetName, DWORD time)
{
	if(lpAnimationSetName == null)
	{
		return false;
	}

	JGEAnimationSet* lpAset = m_lpAnimationSetRoot;
	while(lpAset != null)
	{
		if(jgecsequ(lpAset->m_lpName, lpAnimationSetName))
		{
			m_lpXFileBone->resetHierarchy(m_lpXFileBone->getRootBone());

			int times = time / lpAset->m_timeTotal;
			time = time - times * lpAset->m_timeTotal;
			
			JGEAnimation* lpAnimation = lpAset->m_lpAnimations;
			while(lpAnimation != null)
			{
				D3DXMatrixIdentity(&lpAnimation->m_lpBone->TransformationMatrix);
				for(DWORD i = 0; i < lpAnimation->m_numTransformKeys; ++i)
				{
					JGEAnimationKeyVector* lpAnimationKey = &lpAnimation->m_lpTransformKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numTransformKeys - 1 || time < lpAnimation->m_lpTransformKeys[i + 1].m_time))
					{
						D3DXMatrixTranslation(&lpAnimation->m_lpBone->TransformationMatrix, lpAnimationKey->m_vectorKey.x, lpAnimationKey->m_vectorKey.y, lpAnimationKey->m_vectorKey.z);
						break;
					}
				}
				for (DWORD i = 0; i < lpAnimation->m_numScaleKeys; ++i)
				{
					JGEAnimationKeyVector* lpAnimationKey = &lpAnimation->m_lpScaleKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numScaleKeys - 1 || time < lpAnimation->m_lpScaleKeys[i + 1].m_time))
					{
						D3DXMatrixScaling(&lpAnimation->m_lpBone->TransformationMatrix, lpAnimationKey->m_vectorKey.x, lpAnimationKey->m_vectorKey.y, lpAnimationKey->m_vectorKey.z);
						break;
					}
				}
				for (DWORD i = 0; i < lpAnimation->m_numRotationKeys; ++i)
				{
					JGEAnimationKeyQuaternion* lpAnimationKey = &lpAnimation->m_lpRotationKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numRotationKeys - 1 || time < lpAnimation->m_lpRotationKeys[i + 1].m_time))
					{
						D3DXMatrixRotationQuaternion(&lpAnimation->m_lpBone->TransformationMatrix, &lpAnimationKey->m_quaternionKey);
						break;
					}
				}
				for (DWORD i = 0; i < lpAnimation->m_numMatrixKey; ++i)
				{
					JGEAnimationKeyMatrix* lpAnimationKey = &lpAnimation->m_lpMatrixKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numMatrixKey - 1 || time < lpAnimation->m_lpMatrixKeys[i + 1].m_time))
					{
						lpAnimation->m_lpBone->TransformationMatrix *= lpAnimationKey->m_matrixKey;
						break;
					}
				}

				lpAnimation = lpAnimation->m_lpNextAnimation;
			}

			m_lpXFileBone->updateHierarchy(m_lpXFileBone->getRootBone());

			return true;
		}
	}

	return false;
}

void* JGEXFileAnimation::parseChild(ID3DXFileData* lpXFileData, bool isReference, ID3DXFileData* lpXFileDataParent, void* lpDataParent, GUID* lpGuid)
{
	if(*lpGuid == TID_D3DRMAnimationSet && !isReference)
	{
		JGEAnimationSet* lpAnimationSet = new JGEAnimationSet();
		lpAnimationSet->m_lpNextAnimationSet = m_lpAnimationSetRoot;
		m_lpAnimationSetRoot = lpAnimationSet;

		SIZE_T nameSize = 0;
		char* lpName = null;
		jgeDXVerifyIf(lpXFileData->GetName(null, &nameSize))
			return null;
		jgeDXVerifyEndIf
		lpName = new char[nameSize];
		jgeDXVerifyIf(lpXFileData->GetName(lpName, &nameSize))
			jgeDeleteArray(lpName);
			return null;
		jgeDXVerifyEndIf
		lpAnimationSet->m_lpName = lpName;

		return lpAnimationSet;
	}
	else if(*lpGuid == TID_D3DRMAnimation && !isReference)
	{
		jgeAssert(lpDataParent != null);

		JGEAnimationSet* lpAnimationSet = (JGEAnimationSet*)lpDataParent;
		JGEAnimation* lpAnimation = new JGEAnimation();
		lpAnimation->m_lpAnimationSet = lpAnimationSet;
		++lpAnimationSet->m_numAnimations;
		lpAnimation->m_lpNextAnimation = lpAnimationSet->m_lpAnimations;
		lpAnimationSet->m_lpAnimations = lpAnimation;
		
		return lpAnimation;
	}
	else if(*lpGuid == TID_D3DRMFrame && isReference && lpDataParent != null)
	{
		jgeAssert(lpDataParent != null);

		JGEAnimation* lpAnimation = (JGEAnimation*)lpDataParent;
		SIZE_T nameSize = 0;
		char* lpName = null;
		jgeDXVerifyIf(lpXFileData->GetName(null, &nameSize))
			return null;
		jgeDXVerifyEndIf
		lpName = new char[nameSize];
		jgeDXVerifyIf(lpXFileData->GetName(lpName, &nameSize))
			jgeDeleteArray(lpName);
			return null;
		jgeDXVerifyEndIf
		JGEXFrame* lpBone = m_lpXFileBone->findBone(lpName);
		jgeDeleteArray(lpName);
		jgeAssert(lpBone != null);
		lpAnimation->m_lpBone = lpBone;

		return null;
	}
	else if(*lpGuid == TID_D3DRMAnimationKey && !isReference)
	{
		jgeAssert(lpDataParent != null);

		JGEAnimation* lpAnimation = (JGEAnimation*)lpDataParent;
		SIZE_T memSize = 0;
		const void* lpMem = null;
		jgeDXVerifyIf(lpXFileData->Lock(&memSize, &lpMem))
			return null;
		jgeDXVerifyEndIf
		DWORD* lpMemData = (DWORD*)lpMem;
		DWORD keyType = *lpMemData; ++lpMemData;
		DWORD numKeys = *lpMemData; ++lpMemData;
		switch(keyType)
		{
			// Rotation
			case 0:
			{
				lpAnimation->m_numRotationKeys = numKeys;
				lpAnimation->m_lpRotationKeys = new JGEAnimationKeyQuaternion[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpRotationKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpRotationKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpRotationKeys[i].m_time;
					}
					++lpMemData;

					FLOAT* lpMemDataF = (FLOAT*)lpMemData;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.w = *lpMemDataF; ++lpMemDataF;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.x = *lpMemDataF; ++lpMemDataF;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.y = *lpMemDataF; ++lpMemDataF;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.z = *lpMemDataF; ++lpMemDataF;
					// lpMemData += sizeof(D3DXQUATERNIONG) / sizeof(FLOAT)
					lpMemData += 4;
				}
				break;
			}
			// Scale
			case 1:
			{
				lpAnimation->m_numScaleKeys = numKeys;
				lpAnimation->m_lpScaleKeys = new JGEAnimationKeyVector[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpScaleKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpScaleKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpScaleKeys[i].m_time;
					}
					++lpMemData;

					D3DXVECTOR3* lpMemDataVec = (D3DXVECTOR3*)lpMemData;
					lpAnimation->m_lpScaleKeys[i].m_vectorKey = *lpMemDataVec;
					// lpMemData += sizeof(D3DXVECTOR3) / sizeof(DWORD);
					lpMemData += 3;
				}
				break;
			}
			// Move
			case 2:
			{
				lpAnimation->m_numTransformKeys = numKeys;
				lpAnimation->m_lpTransformKeys = new JGEAnimationKeyVector[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpTransformKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpTransformKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpTransformKeys[i].m_time;
					}
					++lpMemData;

					D3DXVECTOR3* lpMemDataVec = (D3DXVECTOR3*)lpMemData;
					lpAnimation->m_lpTransformKeys[i].m_vectorKey = *lpMemDataVec;
					// lpMemData += sizeof(D3DXVECTOR3) / sizeof(DWORD);
					lpMemData += 3;
				}
				break;
			}
			// Matrix
			case 4:
			{
				lpAnimation->m_numMatrixKey = numKeys;
				lpAnimation->m_lpMatrixKeys = new JGEAnimationKeyMatrix[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpMatrixKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpMatrixKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpMatrixKeys[i].m_time;
					}
					++lpMemData;

					D3DXMATRIX* lpMemDataMatrix = (D3DXMATRIX*)lpMemData;
					lpAnimation->m_lpMatrixKeys[i].m_matrixKey = *lpMemDataMatrix;
					// lpMemData += sizeof(D3DXMATRIX) / sizeof(FLOAT);
					lpMemData += 16;
				}
				break;
			}
			default:
			{
				jgeAssert(false);
			}
		}
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

void JGEXFileAnimation::cleanUp()
{
	jgeDelete(m_lpAnimationSetRoot);
}