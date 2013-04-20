#ifndef __JC_ANIMATIONKEY_MATRIX_H__
#define __JC_ANIMATIONKEY_MATRIX_H__

#include <Windows.h>
#include <d3dx9.h>

class JGEAnimationKeyMatrix
{
public:
	DWORD m_time;
	D3DXMATRIX m_matrixKey;
};

#endif