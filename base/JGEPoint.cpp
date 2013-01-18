#include "JGEPoint.h"

JGEPoint::JGEPoint()
{
	m_x = 0.0f;
	m_y = 0.0f;
}

JGEPoint::JGEPoint(float x, float y)
{
	m_x = x;
	m_y = y;
}

JGEPoint::JGEPoint(const JGEPoint& value)
{
	m_x = value.m_x;
	m_y = value.m_y;
}

JGEPoint::~JGEPoint()
{
	// Do nothing
}