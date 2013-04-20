#ifndef __JC_ANIMATIONSET_H__
#define __JC_ANIMATIONSET_H__

#include <Windows.h>

#include "JGEAnimation.h"
#include "jgeUtil.h"

class JGEAnimationSet
{
public:
	JGEAnimationSet();
	~JGEAnimationSet();

	char* m_lpName;
	DWORD m_timeTotal;
	JGEAnimationSet* m_lpNextAnimationSet;
	DWORD m_numAnimations;
	JGEAnimation* m_lpAnimations;
};

#endif