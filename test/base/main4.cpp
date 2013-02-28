#include <Windows.h>
#include "jge.h"
#include "JGEMatrix2D.h"

#pragma comment(lib, "base.lib")

using namespace jge;

bool setupCallback()
{
	JGEMatrix2D mat;
	jgeMatrix2DScaling(&mat, 0.5f, 0.5f);
	JGEMatrix2D mat2;
	jgeMatrix2DTranslation(&mat2, 50.0f, 40.0f);
	JGEMatrix2D mat3;
	jgeMatrix2DDotProduct(&mat, &mat2, &mat3);

	D3DXMATRIX dxmat;
	D3DXMatrixScaling(&dxmat, 0.5f, 0.5f, 1.0f);
	D3DXMATRIX dxmat2;
	D3DXMatrixTranslation(&dxmat2, 50.0f, 40.0f, 0.0f);
	D3DXMATRIX dxmat3 = dxmat * dxmat2;

	return true;
}

void releaseCallback()
{

}

void frameCallback(uint timeDelta)
{

}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!init(hInstance, setupCallback, releaseCallback, frameCallback, null, null, null, null, -1, -1, 800, 600, true))
	{
		return 0;
	}
	run();

	return 0;
}