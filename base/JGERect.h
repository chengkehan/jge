#ifndef __JGE_RECT_H__
#define __JGE_RECT_H__

#include "jgeUtil.h"

class JGERect
{
public:
	JGERect();
	JGERect(float left, float top, float right, float bottom);
	JGERect(const JGERect& value);
	~JGERect();

	float m_left, m_top, m_right, m_bottom;

	bool contains(float x, float y) const;
	bool contains(const JGERect* lpRect) const;
	bool intersects(const JGERect* lpRect) const;
	JGERect* intersection(const JGERect* lpDestRect, JGERect* lpIntersectionResult) const;
	JGERect* combine(const JGERect* lpDestRect, JGERect* lpCombineResult) const;
	JGERect* clone(JGERect* lpDestRect) const;
	const JGERect* copy(const JGERect* lpSrcRect);
	void zero();

private:
	inline bool containsPoint(float x, float y) const { return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom; }
	inline bool intersectoinInternal(const JGERect* lpRect, float* x1, float* x2, float* y1 ,float* y2) const
	{
		*x1 = max(min(m_left, m_right), min(lpRect->m_left, lpRect->m_right));
		*x2 = min(min(m_left, m_right), min(lpRect->m_left, lpRect->m_right));
		*y1 = max(min(m_top, m_bottom), min(lpRect->m_top, lpRect->m_bottom));
		*y2 = min(min(m_top, m_bottom), min(lpRect->m_top, lpRect->m_bottom));
		return *x1 <= *x2 && *y1 <= *y2;
	}
};

#endif