#ifndef __JGE_RECT_H__
#define __JGE_RECT_H__

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
	bool containsPoint(float x, float y) const;
	bool intersectoinInternal(const JGERect* lpRect, float* x1, float* x2, float* y1 ,float* y2) const;
};

#endif