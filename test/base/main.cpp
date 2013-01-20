#include <Windows.h>
#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

JGEDisplayObject* lpBox;

bool setupCallback();
void releaseCallback();
void frameCallback(uint timeDelta);

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!init(hInstance, setupCallback, releaseCallback, frameCallback))
	{
		return 0;
	}
	run();

	return 0;
}

bool setupCallback()
{
	if(!loadTexture(0, "rect.png"))
	{
		return false;
	}

	lpBox = newDisplayObject();
	lpBox->setTexture(getTexture(0));
	getStage()->addChild(lpBox);

	return true;
}

void releaseCallback()
{
	releaseDisplayObject(lpBox);
}

void frameCallback(uint timeDelta)
{

}