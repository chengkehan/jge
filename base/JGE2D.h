#ifndef __JGE_2D_H__
#define __JGE_2D_H__

#include "JGEAbstractDisplayObject.h"
#include "JGEDisplayObjectContainer.h"
#include "JGEDisplayObject.h"
#include "JGE3D.h"
#include "JGEInput.h"
#include "jgeUtil.h"

class JGE2D
{
JGE_SINGLETON_DECLARE(JGE2D)

public:
	bool init(HINSTANCE hInstance, 
		JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		JGE3D::WMDESTROYCALLBACK wmDestroyCallback = null, JGE3D::WMESCAPEKEYDOWNCALLBACK wmEscapeKeyDownCallback = null, 
		JGE3D::DEVICELOSECALLBACK deviceLoseCallback = null, JGE3D::DEVICELOSERESETCALLBACK deviceLoseResetCallback = null, 
		int windowX = 0, int windowY = 0, uint windowWidth = 800, uint windowHeight = 600, bool windowd = true, bool clientMouse = true);
	bool initManual(HINSTANCE hInstance, HWND hwnd, const D3DVIEWPORT9* lpViewPort, 
		JGE3D::SETUPCALLBACK setupCallback = null, JGE3D::RELEASECALLBACK releaseCallback = null, JGE3D::FRAMECALLBACK frameCallback = null, 
		JGE3D::WMDESTROYCALLBACK wmDestroyCallback = null, JGE3D::WMESCAPEKEYDOWNCALLBACK wmEscapeKeyDownCallback = null, 
		JGE3D::DEVICELOSECALLBACK deviceLoseCallback = null, JGE3D::DEVICELOSERESETCALLBACK deviceLoseResetCallback = null, bool clientMouse = true);
	inline JGEDisplayObjectContainer* getStage() const { return m_lpStage; }
	void setMouseVisible(bool value);
	inline bool getMouseVisible() const { return m_mouseVisible; }
	void setMouseLockOnWindow(bool value);
	inline bool getMouseLockOnWindow() const { return JGEInput::getInstance()->getMouseLockedOnWindow(); }
	inline bool getClientMouse() const { return m_clientMouse; }

	static void jgeResetMouseEvent(JGEAbstractDisplayObject* lpDisplayObject);

private:
	JGE2D();
	~JGE2D();

	JGEDisplayObjectContainer* m_lpStage;
	JGE3D::FRAMECALLBACK m_frameCallback;
	JGE3D::DEVICELOSECALLBACK m_deviceLoseCallback;
	JGE3D::DEVICELOSERESETCALLBACK m_deviceLoseResetCallback;
	bool m_mouseVisible;
	bool m_init;
	bool m_clientMouse;
	
	static JGEAbstractDisplayObject* m_lpMouseLeftButtonTarget;
	static JGEAbstractDisplayObject* m_lpMouseRightButtonTarget;
	static JGEAbstractDisplayObject* m_lpMouseMiddleButtonTarget;
	static JGEAbstractDisplayObject* m_lpMouseAreaTarget;
	static void jgeFrameCallback(uint timeDelta);
	static void jgeMouseLockOnWindowProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam);
	static void jgeUpdateMouseEvent();
	static void jgeDeviceLoseCallback();
	static bool jgeDeviceLoseResetCallback();
};

#endif