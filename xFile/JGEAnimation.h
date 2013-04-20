#ifndef __JC_AMINATION_H__
#define __JC_AMINATION_H__

#include <Windows.h>

#include "JGEXFrame.h"
#include "JGEAnimationKeyMatrix.h"
#include "JGEAnimationKeyQuaternion.h"
#include "JGEAnimationKeyVector.h"
#include "jgeUtil.h"

class JGEAnimationSet;

class JGEAnimation
{
public:
	JGEAnimation();
	~JGEAnimation();

	JGEXFrame* m_lpBone;
	JGEAnimation* m_lpNextAnimation;
	JGEAnimationSet* m_lpAnimationSet;
	
	DWORD m_numTransformKeys;
	JGEAnimationKeyVector* m_lpTransformKeys;

	DWORD m_numScaleKeys;
	JGEAnimationKeyVector* m_lpScaleKeys;

	DWORD m_numRotationKeys;
	JGEAnimationKeyQuaternion* m_lpRotationKeys;

	DWORD m_numMatrixKey;
	JGEAnimationKeyMatrix* m_lpMatrixKeys;
};

#endif