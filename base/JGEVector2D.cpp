#include "JGEVector2D.h"

JGEVector2D::JGEVector2D()
{
	m_x = 0.0f; m_y = 0.0f; m_w = 1.0f;
}

JGEVector2D::~JGEVector2D()
{
	// Do nothing
}

JGEVector2D::JGEVector2D(const JGEVector2D& value)
{
	m_x = value.m_x; m_y = value.m_y; m_w = value.m_w;
}

JGEVector2D::JGEVector2D(float x, float y, float w)
{
	m_x = x; m_y = y; m_w = w;
}