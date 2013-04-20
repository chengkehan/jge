#include "JGEAnimation.h"

JGEAnimation::JGEAnimation()
{
	m_lpBone = null;
	m_lpNextAnimation = null;
	m_numTransformKeys = 0;
	m_lpTransformKeys = null;
	m_numScaleKeys = 0;
	m_lpScaleKeys = null;
	m_numRotationKeys = 0;
	m_lpRotationKeys = null;
	m_numMatrixKey = 0;
	m_lpMatrixKeys = null;
	m_lpAnimationSet = null;
}

JGEAnimation::~JGEAnimation()
{
	m_lpBone = null;
	m_lpAnimationSet = null;
	jgeDeleteArray(m_lpTransformKeys); m_lpTransformKeys = null;
	jgeDeleteArray(m_lpScaleKeys); m_lpScaleKeys = null;
	jgeDeleteArray(m_lpRotationKeys); m_lpRotationKeys = null;
	jgeDeleteArray(m_lpMatrixKeys); m_lpMatrixKeys = null;
	jgeDelete(m_lpNextAnimation); m_lpNextAnimation = null;
}