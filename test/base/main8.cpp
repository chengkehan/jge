#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

bool setupCallback();
void releaseCallback();
void frameCallback(uint timeDelta);
void buttonClickHandler(JGEEvent* lpEvent);

JGEButton* lpButton = null;
JGEDisplayObject* lpOutSkin = null;
JGEDisplayObject* lpOverSkin = null;
JGEDisplayObject* lpDownSkin = null;
JGEDisplayObject* lpDisabledSkin = null;

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
	jgeAssert(loadTexture(0, "out.png") != null);
	jgeAssert(loadTexture(1, "over.png") != null);
	jgeAssert(loadTexture(2, "down.png") != null);
	jgeAssert(loadTexture(3, "disabled.png") != null);

	lpOutSkin = newDisplayObject();
	lpOutSkin->setTexture(getTexture(0));
	lpOverSkin = newDisplayObject();
	lpOverSkin->setTexture(getTexture(1));
	lpDownSkin = newDisplayObject();
	lpDownSkin->setTexture(getTexture(2));
	lpDisabledSkin = newDisplayObject();
	lpDisabledSkin->setTexture(getTexture(3));

	lpButton = newButton();
	lpButton->setOutSkin(lpOutSkin);
	lpButton->setOverSkin(lpOverSkin);
	lpButton->setDownSkin(lpDownSkin);
	lpButton->setDisabledSkin(lpDisabledSkin);
	getStage()->addChild(lpButton);
	lpButton->setX(100);
	lpButton->setY(80);
	//lpButton->setRotation(lpButton->getRotation() + 0.3f);
	//lpButton->setEnabled(false);
	lpButton->addEventListener(JGEEvent::MOUSE_CLICK_LEFT, jgeCallbackStd(buttonClickHandler));

	return true;
}

void buttonClickHandler(JGEEvent* lpEvent)
{
	jgeTrace1("%s\n", "button clicked");
}

void releaseCallback()
{
	releaseButton(lpButton);
	releaseDisplayObject(lpOutSkin);
	releaseDisplayObject(lpOverSkin);
	releaseDisplayObject(lpDownSkin);
	releaseDisplayObject(lpDisabledSkin);
}

void frameCallback(uint timeDelta)
{

}