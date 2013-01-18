#ifndef __JGE_POINT_H__
#define __JGE_POINT_H__

class JGEPoint
{
public:
	JGEPoint();
	JGEPoint(float x, float y);
	JGEPoint(const JGEPoint& value);
	~JGEPoint();

	float m_x, m_y;
};

#endif