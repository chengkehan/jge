#include <Windows.h>
#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

const uint numBoxes = 7;
JGEDisplayObject* lpBoxList[numBoxes];

bool setupCallback();
void releaseCallback();
void frameCallback(uint timeDelta);
void lpBoxMouseOverHandler(JGEEvent* lpEvent);
void lpBoxMouseOutHandler(JGEEvent* lpEvent);
void lpBoxMouseClickLeftHandler(JGEEvent* lpEvent);
void createBox();

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!init(hInstance, setupCallback, releaseCallback, frameCallback, null, null, null, null, -1, -1, 800, 600, true))
	{
		return 0;
	}
	run();

	return 0;
}

void createBox(uint index)
{
	JGEDisplayObject* lpBox = newDisplayObject();
	lpBox->setTexture(getTexture(0));
	lpBox->setX(100.0f);
	lpBox->setY(100.0f + 30.0f * (index + 1));
	lpBox->setRefX(lpBox->getWidth() * 0.5f);
	lpBox->setRefY(lpBox->getHeight() * 0.5f);
	lpBox->addEventListener(JGEEvent::MOUSE_OVER, lpBoxMouseOverHandler);
	lpBox->addEventListener(JGEEvent::MOUSE_OUT, lpBoxMouseOutHandler);
	lpBox->addEventListener(JGEEvent::MOUSE_CLICK_LEFT, lpBoxMouseClickLeftHandler);
	getStage()->addChild(lpBox);
	lpBoxList[index] = lpBox;
}

bool setupCallback()
{
	if(!loadTexture(0, "rect.png"))
	{
		return false;
	}

	srand((unsigned int)time(null));
	for(uint i = 0; i < numBoxes; ++i)
	{
		createBox(i);
	}

	setFPS(60);

	return true;
}

void releaseCallback()
{
	for(uint i = 0; i < numBoxes; ++i)
	{
		releaseDisplayObject(lpBoxList[i]);
	}
}

void frameCallback(uint timeDelta)
{
	for(uint i = 0; i < numBoxes; ++i)
	{
		lpBoxList[i]->setRotation(lpBoxList[i]->getRotation() + 1.0f * timeDelta * 0.001f);
	}
}

void lpBoxMouseOverHandler(JGEEvent* lpEvent)
{
	((JGEDisplayObject*)lpEvent->m_lpEventDispatcher)->setAlpha(0.2f);
}

void lpBoxMouseOutHandler(JGEEvent* lpEvent)
{
	((JGEDisplayObject*)lpEvent->m_lpEventDispatcher)->setAlpha(1.0f);
}

void lpBoxMouseClickLeftHandler(JGEEvent* lpEvent)
{
	((JGEDisplayObject*)lpEvent->m_lpEventDispatcher)->setAlpha(0.6f);
}