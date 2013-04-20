#include "JGEAnimationSet.h"

JGEAnimationSet::JGEAnimationSet()
{
	m_lpName = null;
	m_timeTotal = 0;
	m_lpNextAnimationSet = null;
	m_numAnimations = 0;
	m_lpAnimations = null;
}

JGEAnimationSet::~JGEAnimationSet()
{
	jgeDeleteArray(m_lpName); m_lpName = null;
	jgeDelete(m_lpAnimations); m_lpAnimations = null;
	jgeDelete(m_lpNextAnimationSet); m_lpNextAnimationSet = null;
}