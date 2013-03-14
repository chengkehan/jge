#include <Windows.h>
#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

const uint numBoxes = 1;
JGEDisplayObject* lpBoxList[numBoxes];
JGEText* lpText;
JGEDisplayObjectContainer* lpContainer;

bool setupCallback();
void releaseCallback();
void frameCallback(uint timeDelta);
void createBox();
void mouseOverTextHandler(JGEEvent* lpEvent);
void mouseOutTextHandler(JGEEvent* lpEvent);

typedef struct __CustomHandler
{
	void mouseOverTextHandler(JGEEvent* lpEvent)
	{
		((JGEText*)lpEvent->m_lpEventDispatcher)->setAlpha(0.5f);
	}

	void mouseOutTextHandler(JGEEvent* lpEvent)
	{
		((JGEText*)lpEvent->m_lpEventDispatcher)->setAlpha(1.0f);
	}
} CustomHandler;

CustomHandler customHandler;

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
	lpBox->setY(0.0f);
	lpBox->setRefX(lpBox->getWidth() * 0.5f);
	lpBox->setRefY(lpBox->getHeight() * 0.5f);
	lpContainer->addChild(lpBox);
	lpBoxList[index] = lpBox;
}

bool setupCallback()
{
	if(!loadTexture(0, "rect.png"))
	{
		return false;
	}

	lpContainer = newDisplayObjectContainer();
	getStage()->addChild(lpContainer);
	lpContainer->setX(getStageWidth() * 0.5f);
	lpContainer->setY(getStageHeight() * 0.5f);

	srand((unsigned int)time(null));
	for(uint i = 0; i < numBoxes; ++i)
	{
		createBox(i);

		if(i == 0)
		{
			lpText = newText();
			lpText->setX(100);
			lpText->setY(200);
			lpText->setScaleX(1.5f);
			lpText->setRotation(4.14f);
			//lpText->setAlpha(0.5f);
			//lpText->addEventListener(JGEEvent::MOUSE_OVER, jgeCallbackStd(mouseOverTextHandler));
			//lpText->addEventListener(JGEEvent::MOUSE_OUT, jgeCallbackStd(mouseOutTextHandler));
			lpText->addEventListener<CustomHandler>(JGEEvent::MOUSE_OVER, jgeCallbackThis(&customHandler, &CustomHandler::mouseOverTextHandler));
			lpText->addEventListener<CustomHandler>(JGEEvent::MOUSE_OUT, jgeCallbackThis(&customHandler, &CustomHandler::mouseOutTextHandler));
			getStage()->addChild(lpText);
		}
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
	releaseText(lpText);
	releaseDisplayObjectContainer(lpContainer);
}

void frameCallback(uint timeDelta)
{
	lpContainer->setRotation(lpContainer->getRotation() + 1.0f * timeDelta * 0.001f);
	for(uint i = 0; i < numBoxes; ++i)
	{
		lpBoxList[i]->setRotation(lpBoxList[i]->getRotation() + 1.0f * timeDelta * 0.001f);
	}
	static wchar_t buffer[50];
	wsprintf(buffer, L"FPS:%d", getFPS());
	lpText->setText(buffer);
}

void mouseOverTextHandler(JGEEvent* lpEvent)
{
	((JGEText*)lpEvent->m_lpEventDispatcher)->setAlpha(0.5f);
}

void mouseOutTextHandler(JGEEvent* lpEvent)
{
	((JGEText*)lpEvent->m_lpEventDispatcher)->setAlpha(1.0f);
}