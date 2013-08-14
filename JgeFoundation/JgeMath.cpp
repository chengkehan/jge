#include "JgeStdafx.h"
#include "JgeMath.h"
#include "JgeCommon.h"
#include "JgeMemory.h"

// Point-----------------------------------------------------------------------------------
jge::Point::Point():
	x(0.0f), y(0.0f)
{
	// Do nothing
}

jge::Point::Point(float xValue, float yValue):
	x(xValue), y(yValue)
{
	// Do nothing
}

jge::Point::Point(const Point& value):
	x(value.x), y(value.y)
{
	// Do nothing
}

jge::Point::~Point()
{
	// Do nothing
}

jge::Point& jge::Point::operator=(const jge::Point& value)
{
	x = value.x;
	y = value.y;
	return *this;
}

// Rect-----------------------------------------------------------------------------------
jge::Rect::Rect():
	left(0.0f), top(0.0f), right(0.0f), bottom(0.0f)
{
	// Do nothing
}

jge::Rect::Rect(float leftValue, float topValue, float rightValue, float bottomValue):
	left(leftValue), top(topValue), right(rightValue), bottom(bottomValue)
{
	// Do nothing
}

jge::Rect::Rect(const Rect& value):
	left(value.left), top(value.top), right(value.right), bottom(value.bottom)
{
	// Do nothing
}

jge::Rect::~Rect()
{
	// Do nothing
}

jge::Rect& jge::Rect::operator=(const jge::Rect& value)
{
	left = value.left;
	top = value.top;
	right = value.right;
	bottom = value.bottom;
	return *this;
}

bool jge::Rect::contains(float x, float y)
{
	return containsPoint(x, y);
}

bool jge::Rect::contains(const Rect* lpRect)
{
	return 
		lpRect == null ? false : 
		lpRect == this ? true : 
		containsPoint(lpRect->left, lpRect->top) && containsPoint(lpRect->right, lpRect->bottom);
}

bool jge::Rect::intersects(const Rect* lpRect)
{
	if(lpRect == null)
	{
		return false;
	}
	else if(lpRect == this)
	{
		return true;
	}
	else
	{
		float x1, x2, y1, y2;
		return intersectoinInternal(lpRect, &x1, &x2, &y1, &y2);
	}
}

jge::Rect* jge::Rect::intersection(const jge::Rect* lpDestRect, jge::Rect* lpIntersectionResult)
{
	if(lpIntersectionResult == null)
	{
		return null;
	}
	else if(lpDestRect == this)
	{
		return clone(lpIntersectionResult);
	}
	else
	{
		float x1, x2, y1, y2;
		bool r = intersectoinInternal(lpDestRect, &x1, &x2, &y1, &y2);
		if(!r)
		{
			return null;
		}
		else
		{
			lpIntersectionResult->left = x1;
			lpIntersectionResult->right = x2;
			lpIntersectionResult->top = y1;
			lpIntersectionResult->bottom = y2;
			return lpIntersectionResult;
		}
	}
}

jge::Rect* jge::Rect::combine(const Rect* lpDestRect, Rect* lpCombineResult)
{
	if(lpDestRect == null || lpCombineResult == null)
	{
		return null;
	}
	else if(lpDestRect == this)
	{
		return clone(lpCombineResult);
	}
	else
	{
		float left = this->left;
		float top = this->top;
		float right = this->right;
		float bottom = this->bottom;
		lpCombineResult->left = min(min(left, right), min(lpDestRect->left, lpDestRect->right));
		lpCombineResult->right = max(max(left, right), max(lpDestRect->left, lpDestRect->right));
		lpCombineResult->top = min(min(top, bottom), min(lpDestRect->top, lpDestRect->bottom));
		lpCombineResult->bottom = max(max(top, bottom), max(lpDestRect->top, lpDestRect->bottom));
		return lpCombineResult;
	}
}

jge::Rect* jge::Rect::clone(Rect* lpDestRect)
{
	if(lpDestRect == null)
	{
		return lpDestRect;
	}
	else if(lpDestRect == this)
	{
		return lpDestRect;
	}
	else
	{
		lpDestRect->left = left;
		lpDestRect->top = top;
		lpDestRect->right = right;
		lpDestRect->bottom = bottom;

		return lpDestRect;
	}
}

const jge::Rect* jge::Rect::copy(const Rect* lpSrcRect)
{
	if(lpSrcRect == null)
	{
		return null;
	}
	else if(lpSrcRect == this)
	{
		return lpSrcRect;
	}
	else
	{
		left = lpSrcRect->left;
		top = lpSrcRect->top;
		right = lpSrcRect->right;
		bottom = lpSrcRect->bottom;

		return this;
	}
}

void jge::Rect::zero()
{
	left = 0.0f;
	top = 0.0f;
	right = 0.0f;
	bottom = 0.0f;
}

// Vector2D-----------------------------------------------------------------------------------
jge::Vector2D::Vector2D():
	x(0.0f), y(0.0f), w(0.0f)
{
	// Do nothing
}

jge::Vector2D::Vector2D(const Vector2D& value):
	x(value.x), y(value.y), w(value.w)
{
	// Do nothing
}

jge::Vector2D::Vector2D(float xValue, float yValue, float wValue):
	x(xValue), y(yValue), w(wValue)
{
	// Do nothing
}

jge::Vector2D::~Vector2D()
{
	// Do nothing
}

jge::Vector2D& jge::Vector2D::operator=(const jge::Vector2D& value)
{
	x = value.x;
	y = value.y;
	w = value.w;
	return *this;
}

// Vector3D-----------------------------------------------------------------------------------
jge::Vector3D::Vector3D():
	x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
	// Do nothing
}

jge::Vector3D::Vector3D(const Vector3D& value):
	x(value.x), y(value.y), z(value.z), w(value.w)
{
	// Do nothing
}

jge::Vector3D::Vector3D(float xValue, float yValue, float zValue, float wValue):
	x(xValue), y(yValue), z(zValue), w(wValue)
{
	// Do nothing
}

jge::Vector3D::~Vector3D()
{
	// Do nothing
}

jge::Vector3D& jge::Vector3D::operator=(const jge::Vector3D& value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	w = value.w;
	return *this;
}

// Matrix4x4-----------------------------------------------------------------------------------
jge::Matrix4x4::Matrix4x4():
	m_11(1.0f), m_12(0.0f), m_13(0.0f), m_14(0.0f), 
	m_21(0.0f), m_22(1.0f), m_23(0.0f), m_24(0.0f), 
	m_31(0.0f), m_32(0.0f), m_33(1.0f), m_34(0.0f), 
	m_41(0.0f), m_42(0.0f), m_43(0.0f), m_44(1.0f)
{
	// Do nothing
}

jge::Matrix4x4::Matrix4x4(const Matrix4x4& value)
{
	jgeMemCpy(value.m_data, m_data, sizeof(float) * 16);
}

jge::Matrix4x4::Matrix4x4(
	float m_11, float m_12, float m_13, float m_14, 
	float m_21, float m_22, float m_23, float m_24, 
	float m_31, float m_32, float m_33, float m_34, 
	float m_41, float m_42, float m_43, float m_44
)
{
	this->m_11 = m_11; this->m_12 = m_12; this->m_13 = m_13; this->m_14 = m_14;
	this->m_21 = m_21; this->m_22 = m_22; this->m_23 = m_23; this->m_24 = m_24;
	this->m_31 = m_31; this->m_32 = m_32; this->m_33 = m_33; this->m_34 = m_34;
	this->m_41 = m_41; this->m_42 = m_42; this->m_43 = m_43; this->m_44 = m_44;
}

void jge::Matrix4x4::identity()
{
	jgeZeroMem(m_data, sizeof(float) * 16);
	m_11 = 1.0f; m_22 = 1.0f; m_33 = 1.0f; m_44 = 1.0f;
}

jge::Matrix4x4::~Matrix4x4()
{
	// Do nothing
}

jge::Matrix4x4& jge::Matrix4x4::operator=(const jge::Matrix4x4& value)
{
	jgeMemCpy(value.m_data, m_data, sizeof(float) * 16);
	return *this;
}