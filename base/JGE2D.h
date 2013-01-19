#ifndef __JGE_2D_H__
#define __JGE_2D_H__

#include "JGEDisplayObjectContainer.h"
#include "JGE3D.h"
#include "jgeUtil.h"

class JGE2D
{
JGE_SINGLETON_DECLARE(JGE2D)

public:
	bool init(HINSTANCE hInstance, JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		int windowX = 0, int windowY = 0, uint windowWidth = 800, uint windowHeight = 600, bool windowd = TRUE);
	JGEDisplayObjectContainer* getStage() const;
	void run();
	void setExitWhileEscapeDown(bool value);
	bool getExitWhileEscapeDown() const;
	void setMouseVisible(bool value);
	bool getMouseVisible() const;
	void setMouseLockOnWindow(bool value);
	bool getMouseLockOnWindow() const;

private:
	JGE2D();
	~JGE2D();

	JGEDisplayObjectContainer* m_stage;
	JGE3D::FRAMECALLBACK m_frameCallback;
	JGE3D::SETUPCALLBACK m_setupCallback;
	JGE3D::RELEASECALLBACK m_releaseCallback;
	bool m_exitWhileEscapeDown;
	bool m_mouseVisible;
	
	static void jc2dFrameCallback(uint timeDelta);
	static void jc2dRenderDisplayObjectContainer(JGEDisplayObjectContainer* lpContainer);
	static void jc2dMouseLockOnWindowProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam);
	static void jc2dUpdateMouseEvent(JGEDisplayObjectContainer* lpContainer);
};

#endif