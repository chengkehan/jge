#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "JgeCommon.h"

namespace jge
{
	enum TextureFilter
	{
		TEXTURE_FILTER_NONE, 
		TEXTURE_FILTER_POINT, 
		TEXTURE_FILTER_LINEAR, 
		TEXTURE_FILTER_TRIANGLE
	};

	class JGE_DLL Texture2D
	{
	public:
		Texture2D();
		Texture2D(uint width, uint height, TextureFilter filter);
		~Texture2D();
		
	private:
		Texture2D(const Texture2D& value);
		Texture2D& operator=(const Texture2D& value);

		TextureFilter m_filter;
	};
}