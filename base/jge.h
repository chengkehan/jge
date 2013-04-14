#ifndef __JGE_H__
#define __JGE_H__

#include "JGE3D.h"
#include "JGE2D.h"
#include "JGEInput.h"
#include "JGETexture.h"
#include "JGETextureManager.h"
#include "JGEDisplayObject.h"
#include "JGEDisplayObjectContainer.h"
#include "JGEText.h"
#include "jgeUtil.h"
#include "JGECallback.h"
#include "JGEButton.h"
#include "JGEScale9GridDisplayObject.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DxErr.lib")

namespace jge
{
	extern JGE2D* lp2D;
	extern JGE3D* lp3D;
	extern JGEInput* lpInput;
	extern JGETextureManager* lpTextureManger;

	inline bool init(HINSTANCE hInstance, 
		JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		JGE3D::WMDESTROYCALLBACK wmDestroyCallback = null, JGE3D::WMESCAPEKEYDOWNCALLBACK wmEscapeKeyDownCallback = null, 
		JGE3D::DEVICELOSECALLBACK deviceLoseCallback = null, JGE3D::DEVICERESETCALLBACK deviceLoseResetCallback = null, 
		int windowX = -1, int windowY = -1, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true)
	{
		lp2D = JGE2D::getInstance();
		lp3D = JGE3D::getInstance();
		lpInput = JGEInput::getInstance();
		lpTextureManger = JGETextureManager::getInstance();
		return lp2D->init(hInstance, 
			setupCallback, releaseCallback, frameCallback, 
			wmDestroyCallback, wmEscapeKeyDownCallback, 
			deviceLoseCallback, deviceLoseResetCallback, 
			windowX, windowY, windowWidth, windowHeight, windowd);
	}

	inline bool initManual(HINSTANCE hInstance, HWND hwnd, const D3DVIEWPORT9* lpViewPort, 
		JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		JGE3D::WMDESTROYCALLBACK wmDestroyCallback = null, JGE3D::WMESCAPEKEYDOWNCALLBACK wmEscapeKeyDownCallback = null, 
		JGE3D::DEVICELOSECALLBACK deviceLoseCallback = null, JGE3D::DEVICERESETCALLBACK deviceLoseResetCallback = null)
	{
		lp2D = JGE2D::getInstance();
		lp3D = JGE3D::getInstance();
		lpInput = JGEInput::getInstance();
		lpTextureManger = JGETextureManager::getInstance();
		return lp2D->initManual(hInstance, hwnd, lpViewPort, 
			setupCallback, releaseCallback, frameCallback, 
			wmDestroyCallback, wmEscapeKeyDownCallback, 
			deviceLoseCallback, deviceLoseResetCallback);
	}

	inline JGE3D* getJGE3D()
	{
		return JGE3D::getInstance();
	}

	inline JGE2D* getJGE2D()
	{
		return JGE2D::getInstance();
	}

	inline JGEInput* getJGEInput()
	{
		return JGEInput::getInstance();
	}

	inline void run()
	{
		JGE3D::getInstance()->run();
	}

	inline void runManual()
	{
		JGE3D::getInstance()->runManual();
	}

	inline void releaseManual()
	{
		if(JGE3D::getInstance()->m_releaseCallback != null)
		{
			JGE3D::getInstance()->m_releaseCallback();
		}
	}

	inline bool setWindowTitle(const wchar_t* lpTitle)
	{
		jgeWin32SetWindowText(JGE3D::getInstance()->getHWnd(), lpTitle);
	}

	inline JGETexture* loadTexture(int id, const char* lpPath)
	{
		return JGETextureManager::getInstance()->loadTextureFromFile(id, lpPath, JGE3D::getInstance()->getDirect3DDevice());
	}

	inline JGETexture* getTexture(int id)
	{
		return JGETextureManager::getInstance()->getTexture(id);
	}

	inline JGEDisplayObjectContainer* getStage()
	{
		return JGE2D::getInstance()->getStage();
	}

	inline uint getStageWidth()
	{
		return JGE3D::getInstance()->getWindowWidth();
	}

	inline uint getStageHeight()
	{
		return JGE3D::getInstance()->getWindowHeight();
	}

	inline int getMouseX()
	{
		return JGEInput::getInstance()->getMouseX();
	}

	inline int getMouseY()
	{
		return JGEInput::getInstance()->getMouseY();
	}

	inline int getClientMouseX()
	{
		return JGEInput::getInstance()->getClientMouseX();
	}

	inline int getClientMouseY()
	{
		return JGEInput::getInstance()->getClientMouseY();
	}

	inline bool isKeyDown(int diKeyCode)
	{
		return JGEInput::getInstance()->keyDown(diKeyCode);
	}

	inline void setMouseSpeed(float value)
	{
		JGEInput::getInstance()->setMouseSpeed(value);
	}

	inline float getMouseSpeed()
	{
		return JGEInput::getInstance()->getMouseSpeed();
	}

	inline bool isMouseLeftButtonDown()
	{
		return JGEInput::getInstance()->getMouseLeftButtonDown();
	}

	inline bool isMouseRightButtonDown()
	{
		return JGEInput::getInstance()->getMouseRightButtonDown();
	}

	inline bool isMouseMiddleButtonDown()
	{
		return JGEInput::getInstance()->getMouseMiddleButtonDown();
	}

	inline void setMouseVisible(bool value)
	{
		JGE2D::getInstance()->setMouseVisible(value);
	}

	inline bool getMouseVisible()
	{
		return JGE2D::getInstance()->getMouseVisible();
	}

	inline void setMouseLockOnWindow(bool value)
	{
		JGE2D::getInstance()->setMouseLockOnWindow(value);
	}

	inline bool getMouseLockOnWindow()
	{
		return JGE2D::getInstance()->getMouseLockOnWindow();
	}

	inline void setFPS(uint value)
	{
		JGE3D::getInstance()->setFPS(value);
	}

	inline uint getFPS()
	{
		return JGE3D::getInstance()->getFPS();
	}

	inline JGEDisplayObject* newDisplayObject()
	{
		JGEDisplayObject* lpDisplayObject = null;
		jgeNewArgs1(lpDisplayObject, JGEDisplayObject, JGE3D::getInstance()->getDirect3DDevice());
		return lpDisplayObject;
	}

	inline void releaseDisplayObject(JGEDisplayObject* lpDisplayObject)
	{
		JGE2D::jgeResetMouseEvent(lpDisplayObject);
		jgeDelete(lpDisplayObject);
	}

	inline JGEDisplayObjectContainer* newDisplayObjectContainer()
	{
		JGEDisplayObjectContainer* lpContainer = null;
		jgeNewArgs1(lpContainer, JGEDisplayObjectContainer, JGE3D::getInstance()->getDirect3DDevice());
		return lpContainer;
	}

	inline void releaseDisplayObjectContainer(JGEDisplayObjectContainer* lpContainer)
	{
		jgeDelete(lpContainer);
	}

	inline JGEText* newText()
	{
		JGEText* lpText = null;
		jgeNewArgs1(lpText, JGEText, JGE3D::getInstance()->getDirect3DDevice());
		return lpText;
	}

	inline void releaseText(JGEText* lpText)
	{
		JGE2D::jgeResetMouseEvent(lpText);
		jgeDelete(lpText);
	}

	inline JGEButton* newButton()
	{
		JGEButton* lpButton = null;
		jgeNewArgs1(lpButton, JGEButton, JGE3D::getInstance()->getDirect3DDevice());
		return lpButton;
	}

	inline void releaseButton(JGEButton* lpButton)
	{
		JGE2D::jgeResetMouseEvent(lpButton);
		jgeDelete(lpButton);
	}

	inline JGEScale9GridDisplayObject* newScale9GridDisplayObject()
	{
		JGEScale9GridDisplayObject* lpDisplayobject = null;
		jgeNewArgs1(lpDisplayobject, JGEScale9GridDisplayObject, JGE3D::getInstance()->getDirect3DDevice());
		return lpDisplayobject;
	}

	inline void releaseScale9GridDisplayObject(JGEScale9GridDisplayObject* lpDisplayObject)
	{
		JGE2D::jgeResetMouseEvent(lpDisplayObject);
		jgeDelete(lpDisplayObject);
	}
}

#endif