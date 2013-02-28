#ifndef __JGE_MATRIX2D_H__
#define __JGE_MATRIX2D_H__

class JGEMatrix2D
{
public:
	union
	{
		struct  
		{
			float m_11, m_12, m_13;
			float m_21, m_22, m_23;
			float m_31, m_32, m_33;
		};
		float m_data[3][3];
	};

	JGEMatrix2D();
	~JGEMatrix2D();
	JGEMatrix2D(const JGEMatrix2D& value);
	JGEMatrix2D(
		float _11, float _12, float _13, 
		float _21, float _22, float _23, 
		float _31, float _32, float _33
	);
};

#endif