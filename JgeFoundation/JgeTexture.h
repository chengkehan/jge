#pragma once

#include "JgeCommon.h"
#include "Jge3D.h"
#include "JgeNoncopyable.h"

namespace jge
{
	enum TextureFilter
	{
		TEXTURE_FILTER_NONE, 
		TEXTURE_FILTER_POINT, 
		TEXTURE_FILTER_LINEAR, 
		TEXTURE_FILTER_TRIANGLE
	};

	class JGE_DLL Texture2D : private Noncopyable
	{
	public:
		Texture2D(Jge3D* lpJge3D);
		Texture2D(Jge3D* lpJge3D, uint width, uint height, TextureFilter filter);
		~Texture2D();
		
		inline TextureFilter getFilter() { return m_filter; }
		inline uint getWidth() { return m_width; }
		inline uint getHeight() { return m_height; }


	private:
		TextureFilter m_filter;
		uint m_width;
		uint m_height;
		Jge3D* m_lpJge3D;

		void release();
	};
}