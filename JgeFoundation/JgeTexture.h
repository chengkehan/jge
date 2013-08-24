#pragma once

#include <d3d9.h>

namespace jge
{
	class JGE_DLL JgeTexture
	{
	public:
		JgeTexture();
		~JgeTexture();

	private:
		JgeTexture(const JgeTexture& value);
		JgeTexture& operator=(const JgeTexture& value);

		IDirect3DTexture9* m_lpTexture;
	};
}