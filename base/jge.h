#ifndef __JGE_H__
#define __JGE_H__

#include "JGE3D.h"
#include "JGE2D.h"
#include "JGEInput.h"
#include "JGETexture.h"
#include "JGETextureManager.h"
#include "JGEDisplayObject.h"
#include "JGEDisplayObjectContainer.h"
#include "jgeUtil.h"

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

	inline bool init(HINSTANCE hInstance, JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		int windowX = 0, int windowY = 0, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true)
	{
		lp2D = JGE2D::getInstance();
		lp3D = JGE3D::getInstance();
		lpInput = JGEInput::getInstance();
		lpTextureManger = JGETextureManager::getInstance();
		return lp2D->init(hInstance, setupCallback, releaseCallback, frameCallback, windowX, windowY, windowWidth, windowHeight, windowd);
	}

	inline void run()
	{
		JGE3D::getInstance()->run();
	}

	inline JGETexture* loadTexture(int id, const char* lpPath)
	{
		return JGETextureManager::getInstance()->loadFileTexture(id, lpPath, JGE3D::getInstance()->getDirect3DDevice());
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

	inline void setExitWhileEscapeDown(bool value)
	{
		JGE2D::getInstance()->setExitWhileEscapeDown(value);
	}

	inline bool getExitWhileEscapeDown()
	{
		return JGE2D::getInstance()->getExitWhileEscapeDown();
	}

	inline JGEDisplayObject* newDisplayObject()
	{
		JGEDisplayObject* lpDisplayObject = null;
		jgeNewArgs1(lpDisplayObject, JGEDisplayObject, JGE3D::getInstance()->getDirect3DDevice());
		return lpDisplayObject;
	}

	inline void releaseDisplayObject(JGEDisplayObject* lpDisplayObject)
	{
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
}

#endif