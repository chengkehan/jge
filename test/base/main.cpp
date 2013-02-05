#include <Windows.h>
#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

JGEDisplayObject* lpBox;

bool setupCallback();
void releaseCallback();
void frameCallback(uint timeDelta);
void lpBoxMouseOverHandler(JGEEvent* lpEvent);
void lpBoxMouseOutHandler(JGEEvent* lpEvent);
void lpBoxMouseClickLeftHandler(JGEEvent* lpEvent);

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
	lpBox->setX(100.0f);
	lpBox->setY(100.0f);
	//lpBox->setRefX(lpBox->getWidth() * 0.5f);
	//lpBox->setRefY(lpBox->getHeight() * 0.5f);
	lpBox->setName("jimBox");
	lpBox->addEventListener(JGEEvent::MOUSE_OVER, lpBoxMouseOverHandler);
	lpBox->addEventListener(JGEEvent::MOUSE_OUT, lpBoxMouseOutHandler);
	lpBox->addEventListener(JGEEvent::MOUSE_CLICK_LEFT, lpBoxMouseClickLeftHandler);
	lpBox->setRotation(0.2f);
	getStage()->addChild(lpBox);

	setFPS(60);

	return true;
}

void releaseCallback()
{
	releaseDisplayObject(lpBox);
}

void frameCallback(uint timeDelta)
{
	//lpBox->setRotation(lpBox->getRotation() + 0.05f);
}

void lpBoxMouseOverHandler(JGEEvent* lpEvent)
{
	lpBox->setAlpha(0.5f);
}

void lpBoxMouseOutHandler(JGEEvent* lpEvent)
{
	lpBox->setAlpha(1.0f);
}

void lpBoxMouseClickLeftHandler(JGEEvent* lpEvent)
{
	jgeTrace1("%s\n", "lpBoxOnClick");
}