#pragma once

namespace jge
{
	// Point-----------------------------------------------------------------------------------
	class JGE_DLL Point
	{
	public:
		Point();
		Point(float x, float y);
		Point(const Point& value);
		~Point();

		Point& operator=(const Point& value);

		float x, y;
	};

	// Rect-----------------------------------------------------------------------------------
	class JGE_DLL Rect
	{
	public:
		Rect();
		Rect(float left, float top, float right, float bottom);
		Rect(const Rect& value);
		~Rect();

		Rect& operator=(const Rect& value);

		float left, top, right, bottom;

		bool contains(float x, float y);
		bool contains(const Rect* lpRect);
		bool intersects(const Rect* lpRect);
		Rect* intersection(const Rect* lpDestRect, Rect* lpIntersectionResult);
		Rect* combine(const Rect* lpDestRect, Rect* lpCombineResult);
		Rect* clone(Rect* lpDestRect);
		const Rect* copy(const Rect* lpSrcRect);
		void zero();

	private:
		inline bool containsPoint(float x, float y) const { return x >= left && x <= right && y >= top && y <= bottom; }
		inline bool intersectoinInternal(const Rect* lpRect, float* x1, float* x2, float* y1 ,float* y2)
		{
			*x1 = max(min(left, right), min(lpRect->left, lpRect->right));
			*x2 = min(min(left, right), min(lpRect->left, lpRect->right));
			*y1 = max(min(top, bottom), min(lpRect->top, lpRect->bottom));
			*y2 = min(min(top, bottom), min(lpRect->top, lpRect->bottom));
			return *x1 <= *x2 && *y1 <= *y2;
		}
	};

	// Vector2D-----------------------------------------------------------------------------------
	class JGE_DLL Vector2D
	{
	public:
		float x, y, w;

		Vector2D();
		Vector2D(const Vector2D& value);
		Vector2D(float x, float y, float w);
		~Vector2D();

		Vector2D& operator=(const Vector2D& value);
	};

	// Vector3D-----------------------------------------------------------------------------------
	class JGE_DLL Vector3D
	{
	public:
		float x, y, z, w;

		Vector3D();
		Vector3D(const Vector3D& value);
		Vector3D(float x, float y, float z, float w);
		~Vector3D();

		Vector3D& operator=(const Vector3D& value);
	};

	// Matrix4x4-----------------------------------------------------------------------------------
	class JGE_DLL Matrix4x4
	{
	public:
		union
		{
			struct  
			{
				float m_11, m_12, m_13, m_14;
				float m_21, m_22, m_23, m_24;
				float m_31, m_32, m_33, m_34;
				float m_41, m_42, m_43, m_44;
			};
			float m_data[4][4];
		};

		Matrix4x4();
		Matrix4x4(const Matrix4x4& value);
		Matrix4x4(
			float m_11, float m_12, float m_13, float m_14, 
			float m_21, float m_22, float m_23, float m_24, 
			float m_31, float m_32, float m_33, float m_34, 
			float m_41, float m_42, float m_43, float m_44
		);
		~Matrix4x4();

		Matrix4x4& operator=(const Matrix4x4& value);

		void identity();
	};
}