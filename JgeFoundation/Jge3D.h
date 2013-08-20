#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include "JgeWindow.h"

#define jge3DIfFailed(hr) if(FAILED(hr)){ DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr), hr); DXTRACE_ERR_MSGBOX(DXGetErrorString(hr), hr); DXTRACE_MSG(DXGetErrorDescription(hr)); DXTRACE_ERR(DXGetErrorDescription(hr), hr); 
#define jge3DEndIf }

namespace jge
{
	class JGE_DLL Jge3D
	{
	public:
		Jge3D();
		~Jge3D();

		bool init(Window* lpWindow);

	private:
		Jge3D(const Jge3D& value);
		Jge3D& operator=(const Jge3D& value);

		Window* m_lpWindow;
		IDirect3D9* m_lpd3d;
		IDirect3DDevice9* m_lpd3dd;
		D3DPRESENT_PARAMETERS m_presentParams;

		void release();
	};
}