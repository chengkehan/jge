#include "JGERect.h"
#include "jgeUtil.h"

JGERect::JGERect()
{
	m_left = 0.0f; m_top = 0.0f; m_right = 0.0f; m_bottom = 0.0f;
}

JGERect::JGERect(float left, float top, float right, float bottom)
{
	m_left = left;
	m_top = top;
	m_right = right;
	m_bottom = bottom;
}

JGERect::JGERect(const JGERect& value)
{
	m_left = value.m_left;
	m_top = value.m_top;
	m_right = value.m_right;
	m_bottom = value.m_bottom;
}

JGERect::~JGERect()
{
	// Do nothing
}

bool JGERect::contains(float x, float y) const
{
	return containsPoint(x, y);
}

bool JGERect::contains(const JGERect* lpRect) const
{
	return 
		lpRect == null ? false : 
		lpRect == this ? true : 
		containsPoint(lpRect->m_top, lpRect->m_left) && containsPoint(lpRect->m_right, lpRect->m_bottom);
}

bool JGERect::intersects(const JGERect* lpRect) const
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

JGERect* JGERect::intersection(const JGERect* lpDestRect, JGERect* lpIntersectionResult) const
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
			lpIntersectionResult->m_left = x1;
			lpIntersectionResult->m_right = x2;
			lpIntersectionResult->m_top = y1;
			lpIntersectionResult->m_bottom = y2;
			return lpIntersectionResult;
		}
	}
}

JGERect* JGERect::combine(const JGERect* lpDestRect, JGERect* lpCombineResult) const
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
		float left = m_left;
		float top = m_top;
		float right = m_right;
		float bottom = m_bottom;
		lpCombineResult->m_left = min(min(left, right), min(lpDestRect->m_left, lpDestRect->m_right));
		lpCombineResult->m_right = max(max(left, right), max(lpDestRect->m_left, lpDestRect->m_right));
		lpCombineResult->m_top = min(min(top, bottom), min(lpDestRect->m_top, lpDestRect->m_bottom));
		lpCombineResult->m_bottom = max(max(top, bottom), max(lpDestRect->m_top, lpDestRect->m_bottom));
		return lpCombineResult;
	}
}

JGERect* JGERect::clone(JGERect* lpDestRect) const
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
		lpDestRect->m_left = m_left;
		lpDestRect->m_top = m_top;
		lpDestRect->m_right = m_right;
		lpDestRect->m_bottom = m_bottom;

		return lpDestRect;
	}
}

const JGERect* JGERect::copy(const JGERect* lpSrcRect)
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
		m_left = lpSrcRect->m_left;
		m_top = lpSrcRect->m_top;
		m_right = lpSrcRect->m_right;
		m_bottom = lpSrcRect->m_bottom;

		return this;
	}
}

void JGERect::zero()
{
	m_left = 0.0f;
	m_top = 0.0f;
	m_right = 0.0f;
	m_bottom = 0.0f;
}

inline bool JGERect::containsPoint(float x, float y) const
{
	return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
}

inline bool JGERect::intersectoinInternal(const JGERect* lpRect, float* x1, float* x2, float* y1 ,float* y2) const
{
	*x1 = max(min(m_left, m_right), min(lpRect->m_left, lpRect->m_right));
	*x2 = min(min(m_left, m_right), min(lpRect->m_left, lpRect->m_right));
	*y1 = max(min(m_top, m_bottom), min(lpRect->m_top, lpRect->m_bottom));
	*y2 = min(min(m_top, m_bottom), min(lpRect->m_top, lpRect->m_bottom));

	return *x1 <= *x2 && *y1 <= *y2;
}