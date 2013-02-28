#include "JGEMatrix2D.h"
#include "jgeUtil.h"

JGEMatrix2D::JGEMatrix2D()
{
	jgeMatrix2DIdentity(this);
}

JGEMatrix2D::~JGEMatrix2D()
{
	// Do nothing
}

JGEMatrix2D::JGEMatrix2D(const JGEMatrix2D& value)
{
	jgeMemCpy(value.m_data, m_data, sizeof(float) * 9);
}

JGEMatrix2D::JGEMatrix2D(
	float _11, float _12, float _13, 
	float _21, float _22, float _23, 
	float _31, float _32, float _33
)
{
	m_11 = _11; m_12 = _12; m_13 = _13;
	m_21 = _21; m_22 = _22; m_23 = _23;
	m_31 = _31; m_32 = _32; m_33 = _33;
}