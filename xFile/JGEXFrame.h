#ifndef __JC_XFRAME_H__
#define __JC_XFRAME_H__

#include <Windows.h>
#include <d3dx9.h>

#include "jgeUtil.h"

class JGEXFrame : public D3DXFRAME
{
public:
	JGEXFrame();
	~JGEXFrame();

	D3DXMATRIX matrixCombine;
	D3DXMATRIX matrixOriginal;

private:
	JGEXFrame(const JGEXFrame& value);
};

#endif