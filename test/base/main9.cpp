#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

bool setupCallback();
void releaseCallback();
void frameCallback(uint timeDelta);
void buttonClickHandler(JGEEvent* lpEvent);
void createButton(int index);

const int NUM_BUTTONS = 200;
JGEButton* buttonList[NUM_BUTTONS];
JGEDisplayObject* skinList[NUM_BUTTONS * 4];
JGEText* lpLabel = null;

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!init(hInstance, setupCallback, releaseCallback, frameCallback, null, null, null, null, -1, -1, 800, 600, true))
	{
		return 0;
	}
	run();

	return 0;
}

void createButton(int index)
{
	JGEDisplayObject* lpOutSkin = newDisplayObject();
	lpOutSkin->setTexture(getTexture(0));
	skinList[index * 4] = lpOutSkin;
	JGEDisplayObject* lpOverSkin = newDisplayObject();
	lpOverSkin->setTexture(getTexture(1));
	skinList[index * 4 + 1] = lpOverSkin;
	JGEDisplayObject* lpDownSkin = newDisplayObject();
	lpDownSkin->setTexture(getTexture(2));
	skinList[index * 4 + 2] = lpDownSkin;
	JGEDisplayObject* lpDisabledSkin = newDisplayObject();
	lpDisabledSkin->setTexture(getTexture(3));
	skinList[index * 4 + 3] = lpDisabledSkin;

	JGEButton* lpButton = newButton();
	lpButton->setOutSkin(lpOutSkin);
	lpButton->setOverSkin(lpOverSkin);
	lpButton->setDownSkin(lpDownSkin);
	lpButton->setDisabledSkin(lpDisabledSkin);
	getStage()->addChild(lpButton);
	lpButton->setX((float)(rand() % (getStageWidth() - 215)));
	lpButton->setY((float)(rand() % (getStageHeight() - 45)));
	//lpButton->setLabel(L"看见没");
	lpButton->setLabelColor(0xFF0000);
	lpButton->setLabelConfig(7, 20, 300, false, L"华文行楷");
	//lpButton->addEventListener(JGEEvent::MOUSE_CLICK_LEFT, jgeCallbackStd(buttonClickHandler));
	buttonList[index] = lpButton;
}

bool setupCallback()
{
	jgeAssert(loadTexture(0, "out.png") != null);
	jgeAssert(loadTexture(1, "over.png") != null);
	jgeAssert(loadTexture(2, "down.png") != null);
	jgeAssert(loadTexture(3, "disabled.png") != null);

	srand((uint)time(null));
	for(int i = 0; i < NUM_BUTTONS; ++i)
	{
		createButton(i);
	}

	lpLabel = newText();
	getStage()->addChild(lpLabel);

	setFPS(60);

	return true;
}

void buttonClickHandler(JGEEvent* lpEvent)
{
	jgeTrace1("%s\n", "button clicked");
}

void releaseCallback()
{
	for(int i = 0; i < NUM_BUTTONS; ++i)
	{
		releaseButton(buttonList[i]);
		releaseDisplayObject(skinList[i * 4]);
		releaseDisplayObject(skinList[i * 4 + 1]);
		releaseDisplayObject(skinList[i * 4 + 2]);
		releaseDisplayObject(skinList[i * 4 + 3]);
	}

	releaseText(lpLabel);
}

void frameCallback(uint timeDelta)
{
	static wchar_t buffer[50];
	wsprintf(buffer, L"FPS:%d\n", getFPS());
	lpLabel->setText(buffer);
}