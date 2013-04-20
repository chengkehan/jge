#include "JGEXFrame.h"

JGEXFrame::JGEXFrame()
{
	Name = null;
	pMeshContainer = null;
	pFrameSibling = null;
	pFrameFirstChild = null;
	D3DXMatrixIdentity(&matrixCombine);
	D3DXMatrixIdentity(&matrixOriginal);
	D3DXMatrixIdentity(&TransformationMatrix);
}

JGEXFrame::~JGEXFrame()
{
	delete []Name;
	Name = null;
	delete pFrameSibling;
	pFrameSibling = null;
	delete pFrameFirstChild;
	pFrameFirstChild = null;
}