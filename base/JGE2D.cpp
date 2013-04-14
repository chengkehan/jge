#include "JGE2D.h"
#include "JGETextureManager.h"
#include "JGERender.h"
#include "JGEDisplayObject.h"
#include "JGE2DQtree.h"
#include "JGEText.h"
#include "JGEAbstractDisplayObject.h"

JGE_SINGLETON_IMPLEMENTS(JGE2D)

JGEAbstractDisplayObject* JGE2D::m_lpMouseLeftButtonTarget = null;
JGEAbstractDisplayObject* JGE2D::m_lpMouseRightButtonTarget = null;
JGEAbstractDisplayObject* JGE2D::m_lpMouseMiddleButtonTarget = null;
JGEAbstractDisplayObject* JGE2D::m_lpMouseAreaTarget = null;

JGE2D::JGE2D()
{
	m_lpStage = null;
	m_mouseVisible = true;
	m_frameCallback = null;
	m_init = false;
}

JGE2D::~JGE2D()
{
	jgeDelete(m_lpStage);
	m_frameCallback = null;
}

bool JGE2D::init(HINSTANCE hInstance, 
	JGE3D::SETUPCALLBACK setupCallback, JGE3D::RELEASECALLBACK releaseCallback, JGE3D::FRAMECALLBACK frameCallback, 
	JGE3D::WMDESTROYCALLBACK wmDestroyCallback, JGE3D::WMESCAPEKEYDOWNCALLBACK wmEscapeKeyDownCallback, 
	JGE3D::DEVICELOSECALLBACK deviceLoseCallback, JGE3D::DEVICERESETCALLBACK deviceResetCallback, 
	int windowX, int windowY, uint windowWidth, uint windowHeight, bool windowd, bool clientMouse)
{
	if(m_init)
	{
		return true;
	}

	JGE3D::getInstance()->m_setupCallback = null;
	JGE3D::getInstance()->m_releaseCallback = releaseCallback;
	JGE3D::getInstance()->m_frameCallback = jgeFrameCallback;
	JGE3D::getInstance()->m_wmDestroyCallback = wmDestroyCallback;
	JGE3D::getInstance()->m_wmEscapeKeyDownCallback = wmEscapeKeyDownCallback;
	JGE3D::getInstance()->m_deviceLoseCallback = jgeDeviceLoseCallback;
	JGE3D::getInstance()->m_deviceResetCallback = jgeDeviceResetCallback;
	m_frameCallback = frameCallback;
	m_deviceLoseCallback = deviceLoseCallback;
	m_deviceResetCallback = deviceResetCallback;

	if(!JGE3D::getInstance()->init(hInstance, windowX, windowY, windowWidth, windowHeight, windowd))
	{
		return false;
	}

	m_clientMouse = clientMouse;
	if(!JGEInput::getInstance()->initInput(hInstance, JGE3D::getInstance()->getHWnd()))
	{
		return false;
	}
	if(!JGERender::getInstance()->init(JGE3D::getInstance()->getDirect3DDevice()))
	{
		return false;
	}

	JGE2DQtree::getInstance()->init(7, windowWidth, windowHeight);

	jgeNewArgs1(m_lpStage, JGEDisplayObjectContainer, JGE3D::getInstance()->getDirect3DDevice());
	m_lpStage->setName("jge2dstage");
	m_lpStage->m_depth = 0;
	m_lpStage->m_index = 0;

	if(setupCallback != null)
	{
		if(!setupCallback())
		{
			return false;
		}
	}

	m_init = true;
	return true;
}

bool JGE2D::initManual(HINSTANCE hInstance, HWND hwnd, const D3DVIEWPORT9* lpViewPort, 
	JGE3D::SETUPCALLBACK setupCallback, JGE3D::RELEASECALLBACK releaseCallback, JGE3D::FRAMECALLBACK frameCallback, 
	JGE3D::WMDESTROYCALLBACK wmDestroyCallback, JGE3D::WMESCAPEKEYDOWNCALLBACK wmEscapeKeyDownCallback, 
	JGE3D::DEVICELOSECALLBACK deviceLoseCallback, JGE3D::DEVICERESETCALLBACK deviceLoseResetCallback, bool clientMouse)
{
	if(m_init)
	{
		return true;
	}

	JGE3D::getInstance()->m_setupCallback = null;
	JGE3D::getInstance()->m_releaseCallback = releaseCallback;
	JGE3D::getInstance()->m_frameCallback = jgeFrameCallback;
	JGE3D::getInstance()->m_wmDestroyCallback = wmDestroyCallback;
	JGE3D::getInstance()->m_wmEscapeKeyDownCallback = wmEscapeKeyDownCallback;
	JGE3D::getInstance()->m_deviceLoseCallback = jgeDeviceLoseCallback;
	JGE3D::getInstance()->m_deviceResetCallback = jgeDeviceResetCallback;
	m_frameCallback = frameCallback;
	m_deviceLoseCallback = deviceLoseCallback;
	m_deviceResetCallback = deviceLoseResetCallback;

	if(!JGE3D::getInstance()->initManual(hInstance, hwnd, lpViewPort))
	{
		return false;
	}

	m_clientMouse = clientMouse;
	if(!JGEInput::getInstance()->initInput(hInstance, JGE3D::getInstance()->getHWnd()))
	{
		return false;
	}
	if(!JGERender::getInstance()->init(JGE3D::getInstance()->getDirect3DDevice()))
	{
		return false;
	}

	JGE2DQtree::getInstance()->init(7, lpViewPort->Width, lpViewPort->Height);

	jgeNewArgs1(m_lpStage, JGEDisplayObjectContainer, JGE3D::getInstance()->getDirect3DDevice());
	m_lpStage->setName("jge2dstage");
	m_lpStage->m_depth = 0;
	m_lpStage->m_index = 0;

	if(setupCallback != null)
	{
		if(!setupCallback())
		{
			return false;
		}
	}

	m_init = true;
	return true;
}

void JGE2D::setMouseVisible(bool value)
{
	m_mouseVisible = value;
	if(m_mouseVisible)
	{
		jgeWin32CursorShow();
	}
	else
	{
		jgeWin32CursorHide();
	}
}

void JGE2D::setMouseLockOnWindow(bool value)
{
	if(value)
	{
		JGE3D::getInstance()->setMessageCallback(WM_MOVE, jgeMouseLockOnWindowProc);
		jgeMouseLockOnWindowProc(null, 0, 0, 0);
	}
	else
	{
		JGE3D::getInstance()->clearMessageCallback(WM_MOVE);
		JGEInput::getInstance()->mouseUnlockOnWindow();
	}
}

void JGE2D::jgeFrameCallback(uint timeDelta)
{
	JGEInput::getInstance()->updateInput();

	if(JGE2D::getInstance()->m_frameCallback != null)
	{
		JGE2D::getInstance()->m_frameCallback(timeDelta);
	}

	static JGEMatrix2D matrixGlobal;
	matrixGlobal.m_13 = JGE2D::getInstance()->getStage()->getAlpha();
	JGE2D::getInstance()->getStage()->updateMatrixGlobal(&matrixGlobal);

	JGE2D::getInstance()->getStage()->qtreeSetClear();

	jgeUpdateMouseEvent();

	JGE3D::getInstance()->getDirect3DDevice()->SetRenderState(D3DRS_ZENABLE, false);
	JGERender::getInstance()->beginScene();
	JGE2D::getInstance()->getStage()->render();
	JGERender::getInstance()->endScene();
}

void JGE2D::jgeMouseLockOnWindowProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	JGEInput::getInstance()->mouseLockOnWindow();
}

void JGE2D::jgeUpdateMouseEvent()
{
	static JGEPoint pointBounds[4];
	static JGERect rect;
	float mouseX = JGE2D::getInstance()->m_clientMouse ? (float)JGEInput::getInstance()->getClientMouseX() : (float)JGEInput::getInstance()->getMouseX();
	float mouseY = JGE2D::getInstance()->m_clientMouse ? (float)JGEInput::getInstance()->getClientMouseY() : (float)JGEInput::getInstance()->getMouseY();
	JGEQtreeNodeData* lpNodeData = JGE2DQtree::getInstance()->getQtree()->search((float)mouseX, (float)mouseY);
	JGEAbstractDisplayObject* lpDisplayObjectResult = null;
	uint depthMax = 0;
	uint indexMax = 0;
	while(lpNodeData != null)
	{
		JGEAbstractDisplayObject* lpDisplayObject = (JGEAbstractDisplayObject*)lpNodeData;
		if(lpDisplayObject->inBoundsGlobal(mouseX, mouseY) && lpDisplayObject->m_depth >= depthMax && lpDisplayObject->m_index >= indexMax)
		{
			depthMax = lpDisplayObject->m_depth;
			indexMax = lpDisplayObject->m_index;
			lpDisplayObjectResult = lpDisplayObject;
		}
		lpNodeData = lpNodeData->m_lpNodeDataNext;
	}

	static bool mouseLeftButtonDown = JGEInput::getInstance()->getMouseLeftButtonDown();
	static bool mouseRightButtonDown = JGEInput::getInstance()->getMouseRightButtonDown();
	static bool mouseMiddleButtonDown = JGEInput::getInstance()->getMouseMiddleButtonDown();
	static JGEEvent evt(0); evt.m_lpEventDispatcher = null;
	bool mouseLeftButtonDownCurrent = JGEInput::getInstance()->getMouseLeftButtonDown();
	bool mouseRightButtonDownCurrent = JGEInput::getInstance()->getMouseRightButtonDown();
	bool mouseMiddleButtonDownCurrent = JGEInput::getInstance()->getMouseMiddleButtonDown();
	if(lpDisplayObjectResult != null)
	{
		// mouse down on target
		if(!mouseLeftButtonDown && mouseLeftButtonDownCurrent)
		{
			evt.m_id = JGEEvent::MOUSE_DOWN_LEFT;
			evt.m_lpEventDispatcher = lpDisplayObjectResult;
			lpDisplayObjectResult->dispatchEvent(&evt);
			m_lpMouseLeftButtonTarget = lpDisplayObjectResult;
		}
		if(!mouseRightButtonDown && mouseRightButtonDownCurrent)
		{
			evt.m_id = JGEEvent::MOUSE_DOWN_RIGHT;
			evt.m_lpEventDispatcher = lpDisplayObjectResult;
			lpDisplayObjectResult->dispatchEvent(&evt);
			m_lpMouseRightButtonTarget = lpDisplayObjectResult;
		}
		if(!mouseMiddleButtonDown && mouseMiddleButtonDownCurrent)
		{
			evt.m_id = JGEEvent::MOUSE_DOWN_MIDDLE;
			evt.m_lpEventDispatcher = lpDisplayObjectResult;
			lpDisplayObjectResult->dispatchEvent(&evt);
			m_lpMouseMiddleButtonTarget = lpDisplayObjectResult;
		}
		// mouse up on target
		if(mouseLeftButtonDown && !mouseLeftButtonDownCurrent)
		{
			evt.m_id = JGEEvent::MOUSE_UP_LEFT;
			evt.m_lpEventDispatcher = lpDisplayObjectResult;
			lpDisplayObjectResult->dispatchEvent(&evt);

			// mouse click on target
			if(lpDisplayObjectResult == m_lpMouseLeftButtonTarget)
			{
				evt.m_id = JGEEvent::MOUSE_CLICK_LEFT;
				evt.m_lpEventDispatcher = lpDisplayObjectResult;
				lpDisplayObjectResult->dispatchEvent(&evt);
				m_lpMouseLeftButtonTarget = lpDisplayObjectResult;
			}
		}
		if(mouseRightButtonDown && !mouseRightButtonDownCurrent)
		{
			evt.m_id = JGEEvent::MOUSE_UP_RIGHT;
			evt.m_lpEventDispatcher = lpDisplayObjectResult;
			lpDisplayObjectResult->dispatchEvent(&evt);

			// mouse click on target
			if(lpDisplayObjectResult == m_lpMouseRightButtonTarget)
			{
				evt.m_id = JGEEvent::MOUSE_CLICK_RIGHT;
				evt.m_lpEventDispatcher = lpDisplayObjectResult;
				lpDisplayObjectResult->dispatchEvent(&evt);
				m_lpMouseRightButtonTarget = lpDisplayObjectResult;
			}
		}
		if(mouseMiddleButtonDown && !mouseMiddleButtonDownCurrent)
		{
			evt.m_id = JGEEvent::MOUSE_UP_MIDDLE;
			evt.m_lpEventDispatcher = lpDisplayObjectResult;
			lpDisplayObjectResult->dispatchEvent(&evt);

			// mouse click on target
			if(lpDisplayObjectResult == m_lpMouseMiddleButtonTarget)
			{
				evt.m_id = JGEEvent::MOUSE_CLICK_MIDDLE;
				evt.m_lpEventDispatcher = lpDisplayObjectResult;
				lpDisplayObjectResult->dispatchEvent(&evt);
				m_lpMouseMiddleButtonTarget = lpDisplayObjectResult;
			}
		}
	}
	else
	{
		m_lpMouseLeftButtonTarget = null;
		m_lpMouseRightButtonTarget = null;
		m_lpMouseMiddleButtonTarget = null;
	}

	if(m_lpMouseAreaTarget != lpDisplayObjectResult)
	{
		if(m_lpMouseAreaTarget != null)
		{
			evt.m_id = JGEEvent::MOUSE_OUT;
			evt.m_lpEventDispatcher = m_lpMouseAreaTarget;
			m_lpMouseAreaTarget->dispatchEvent(&evt);
		}
		if(lpDisplayObjectResult != null)
		{
			evt.m_id = JGEEvent::MOUSE_OVER;
			evt.m_lpEventDispatcher = lpDisplayObjectResult;
			lpDisplayObjectResult->dispatchEvent(&evt);
		}
	}
	m_lpMouseAreaTarget = lpDisplayObjectResult;

	mouseLeftButtonDown = mouseLeftButtonDownCurrent;
	mouseRightButtonDown = mouseRightButtonDownCurrent;
	mouseMiddleButtonDown = mouseMiddleButtonDownCurrent;
}

void JGE2D::jgeResetMouseEvent(JGEAbstractDisplayObject* lpDisplayObject)
{
	if(lpDisplayObject == m_lpMouseLeftButtonTarget)
	{
		m_lpMouseLeftButtonTarget = null;
	}
	if(lpDisplayObject == m_lpMouseRightButtonTarget)
	{
		m_lpMouseRightButtonTarget = null;
	}
	if(lpDisplayObject == m_lpMouseMiddleButtonTarget)
	{
		m_lpMouseMiddleButtonTarget = null;
	}
	if(lpDisplayObject == m_lpMouseAreaTarget)
	{
		m_lpMouseAreaTarget = null;
	}
}

void JGE2D::jgeDeviceLoseCallback()
{
	JGERender::getInstance()->deviceLose();
	JGETextureManager::getInstance()->deviceLose();

	if(JGE2D::getInstance()->m_deviceLoseCallback != null)
	{
		JGE2D::getInstance()->m_deviceLoseCallback();
	}
}

bool JGE2D::jgeDeviceResetCallback()
{
	if(!JGETextureManager::getInstance()->deviceReset())
	{
		return false;
	}

	if(!JGERender::getInstance()->deviceReset())
	{
		return false;
	}

	if(JGE2D::getInstance()->m_deviceResetCallback != null && !JGE2D::getInstance()->m_deviceResetCallback())
	{
		return false;
	}

	return true;
}