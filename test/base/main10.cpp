#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

bool setupCallback();
void releaseCallback();
void frameCallback(uint timeDelta);

JGEScale9GridDisplayObject* lpDisplay = null;

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!init(hInstance, setupCallback, releaseCallback, frameCallback, null, null, null, null, -1, -1, 800, 600, true))
	{
		return 0;
	}
	run();

	return 0;
}

bool setupCallback()
{
	jgeAssert(loadTexture(0, "skin.png") != null);

	lpDisplay = newScale9GridDisplayObject();
	lpDisplay->setTexture(getTexture(0));
	JGERect rect(10.0f, 10.0f, 10.0f, 10.0f);
	lpDisplay->setScale9Grid(&rect);
	//lpDisplay->setScaleX(4.0f);
	//lpDisplay->setScaleY(4.0f);
	//lpDisplay->setRotation(0.5f);
	lpDisplay->setX(getStageWidth() * 0.5f);
	lpDisplay->setY(getStageHeight() * 0.5f);
	//lpDisplay->setRefX(38.0f);
	//lpDisplay->setRefY(12.0f);
	getStage()->addChild(lpDisplay);
	//JGE3D::getInstance()->getDirect3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//JGE3D::getInstance()->getDirect3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	return true;
}

void releaseCallback()
{
	releaseScale9GridDisplayObject(lpDisplay);
}

void frameCallback(uint timeDelta)
{
	//lpDisplay->setRotation(lpDisplay->getRotation() + 0.01f);
	//lpDisplay->setWidth(lpDisplay->getWidth() + 1);
	//lpDisplay->setHeight(lpDisplay->getHeight() + 1);
	lpDisplay->setWidth((float)getClientMouseX() - lpDisplay->getX());
	lpDisplay->setHeight((float)getClientMouseY() - lpDisplay->getY());
}