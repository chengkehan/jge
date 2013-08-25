#pragma once

#include <d3d9.h>

namespace jge
{
	class JGE_DLL Texture
	{
	public:
		Texture();
		~Texture();

		bool load(const wchar_t* lpFileURL);
		bool load(const char* lpBytesData);

	private:
		Texture(const Texture& value);
		Texture& operator=(const Texture& value);

		IDirect3DTexture9* m_lpTexture;
		wchar_t* m_lpFileURL;
		char* m_lpBytesData;

		void release();
	};
}