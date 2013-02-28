#ifndef __JGE_VECTOR2D_H__
#define __JGE_VECTOR2D_H__

class JGEVector2D
{
public:
	float m_x, m_y, m_w;

	JGEVector2D();
	~JGEVector2D();
	JGEVector2D(const JGEVector2D& value);
	JGEVector2D(float x, float y, float w);
};

#endif