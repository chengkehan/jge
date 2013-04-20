#include <Windows.h>

#include "jge.h"
#include "JGEXModel.h"

#pragma comment(lib, "base.lib")
#pragma comment(lib, "xFile.lib")

using namespace jge;

JGEXModel* lpModel = null;

bool setupCallback()
{
	uint fileDataBytes = 0;
	if(!jgeReadFile("tiny.x", null, &fileDataBytes, false))
	{
		return false;
	}
	CHAR* lpFileData = new CHAR[fileDataBytes];
	if(!jgeReadFile("tiny.x", lpFileData, null, false))
	{
		jgeDeleteArray(lpFileData);
		return false;
	}

	lpModel = new JGEXModel(getJGE3D()->getDirect3DDevice());
	if(!lpModel->parse(lpFileData))
	{
		return false;
	}

	jgeDeleteArray(lpFileData);

	jgeSetProjectionPerspectiveTransform(getJGE3D()->getDirect3DDevice(), 800, 600);
	jgeSetViewTransform(getJGE3D()->getDirect3DDevice(), 0.0f, 0.0f, -300.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jgeInitRenderState(getJGE3D()->getDirect3DDevice(), D3DCULL_CCW, FALSE);

	return true;
}

void releaseCallback()
{
	jgeDelete(lpModel);
}

void frameCallback(uint timeDelta)
{
	static DWORD time = 0;
	lpModel->getAnimation()->play("", time);
	time += 100;
	lpModel->draw();

	static FLOAT radius = 300.0f;
	static FLOAT angle = 0.0f;
	jgeSetViewTransform(getJGE3D()->getDirect3DDevice(), cosf(angle) * radius, 0.0f, sinf(angle) * radius, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	angle += 0.01f;
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!init(hInstance, setupCallback, releaseCallback, frameCallback))
	{
		return 0;
	}
	run();

	return 0;
}