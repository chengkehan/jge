#include "JGE2D.h"
#include "JGETextureManager.h"
#include "JGERender.h"
#include "JGEDisplayObject.h"
#include "JGE2DQtree.h"

JGE_SINGLETON_IMPLEMENTS(JGE2D)

JGE2D::JGE2D()
{
	m_lpStage = null;
	m_exitWhileEscapeDown = true;
	m_mouseVisible = true;
	m_frameCallback = null; m_setupCallback = null; m_releaseCallback = null;
	m_init = false;
}

JGE2D::~JGE2D()
{
	if(m_releaseCallback != null)
	{
		m_releaseCallback();
	}
	jgeDelete(m_lpStage);
	m_frameCallback = null; m_setupCallback = null; m_releaseCallback = null;
}

bool JGE2D::init(HINSTANCE hInstance, JGE3D::SETUPCALLBACK setupCallback, JGE3D::RELEASECALLBACK releaseCallback, JGE3D::FRAMECALLBACK frameCallback, 
	int windowX, int windowY, uint windowWidth, uint windowHeight, bool windowd, bool clientMouse)
{
	if(m_init)
	{
		return true;
	}

	JGE3D::getInstance()->setupCallback = null;
	JGE3D::getInstance()->releaseCallback = null;
	JGE3D::getInstance()->frameCallback = jgeFrameCallback;
	m_frameCallback = frameCallback;
	m_setupCallback = setupCallback;
	m_releaseCallback = releaseCallback;

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

	if(m_setupCallback != null)
	{
		if(!m_setupCallback())
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
	if(JGE2D::getInstance()->m_exitWhileEscapeDown && JGEInput::getInstance()->keyDown(DIK_ESCAPE))
	{
		jgeWin32Exit();
		return;
	}

	jgeUpdateQtree(JGE2D::getInstance()->getStage());
	jgeUpdateMouseEvent();
	if(JGE2D::getInstance()->m_frameCallback != null)
	{
		JGE2D::getInstance()->m_frameCallback(timeDelta);
	}

	JGERender::getInstance()->beginScene();
	jgeRenderDisplayObjectContainer(JGE2D::getInstance()->getStage());
	JGERender::getInstance()->endScene();
}

void JGE2D::jgeRenderDisplayObjectContainer(JGEDisplayObjectContainer* lpContainer)
{
	if(lpContainer == null)
	{
		return;
	}
	else
	{
		JGEDisplayObjectContainer::ChildrenList* lpChildren = &lpContainer->m_childrenList;
		for(JGEDisplayObjectContainer::ChildrenList::iterator iter = lpChildren->begin(); iter != lpChildren->end(); ++iter)
		{
			JGEDisplayObject* lpChild = *iter;
			if(lpChild->m_isContainer)
			{
				jgeRenderDisplayObjectContainer((JGEDisplayObjectContainer*)lpChild);
			}
			else
			{
				if(lpChild->m_lpTexture != null)
				{
					lpChild->updateVertexBufferData();
					JGERender::getInstance()->renderDisplayObject(lpChild);
				}
			}
		}
	}
}

void JGE2D::jgeMouseLockOnWindowProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	JGEInput::getInstance()->mouseLockOnWindow();
}

void JGE2D::jgeUpdateMouseEvent()
{
	int mouseX = JGE2D::getInstance()->m_clientMouse ? JGEInput::getInstance()->getClientMouseX() : JGEInput::getInstance()->getMouseX();
	int mouseY = JGE2D::getInstance()->m_clientMouse ? JGEInput::getInstance()->getClientMouseY() : JGEInput::getInstance()->getMouseY();
	JGEQtreeNodeData* lpNodeData = JGE2DQtree::getInstance()->getQtree()->search((float)mouseX, (float)mouseY);
	JGEDisplayObject* lpDisplayObjectResult1 = null;
	static JGEDisplayObject* lpDisplayObjectResult2 = null;
	uint depthMax = 0;
	uint indexMax = 0;
	JGERect rect;
	while(lpNodeData != null)
	{
		JGEDisplayObject* lpDisplayObject = (JGEDisplayObject*)lpNodeData;
		if(!lpDisplayObject->m_isContainer)
		{
			if(lpDisplayObject->getBounds(&rect)->contains((float)mouseX, (float)mouseY) && 	lpDisplayObject->m_depth >= depthMax && lpDisplayObject->m_index >= indexMax)
			{
				depthMax = lpDisplayObject->m_depth;
				indexMax = lpDisplayObject->m_index;
				lpDisplayObjectResult1 = lpDisplayObject;
			}
		}
		lpNodeData = lpNodeData->m_lpNodeDataNext;
	}

	if(lpDisplayObjectResult1 != lpDisplayObjectResult2)
	{
		if(lpDisplayObjectResult2 != null)
		{
			lpDisplayObjectResult2->setAlpha(1.0f);
		}
	}

	if(lpDisplayObjectResult1 != null)
	{
		//static JGERect jgerect;
		//lpDisplayObjectResult->getBounds(&jgerect);
		//jgeTrace2("%f %f ", jgerect.m_left, jgerect.m_top);
		//jgeTrace2("%f %f\n", jgerect.m_right, jgerect.m_bottom);
		//jgeTrace2("%d %d\n", mouseX, mouseY);
		lpDisplayObjectResult1->setAlpha(0.3f);
	}
	lpDisplayObjectResult2 = lpDisplayObjectResult1;
}

void JGE2D::jgeUpdateQtree(JGEDisplayObjectContainer* lpContainer)
{
	if(lpContainer == null)
	{
		return;
	}
	else
	{
		JGERect rect;
		JGEDisplayObjectContainer::ChildrenList* lpChildren = &lpContainer->m_childrenList;
		for(JGEDisplayObjectContainer::ChildrenList::iterator iter = lpChildren->begin(); iter != lpChildren->end(); ++iter)
		{
			JGEDisplayObject* lpChildren = *iter;
			if(lpChildren->m_isContainer)
			{
				jgeUpdateQtree((JGEDisplayObjectContainer*)lpContainer);
			}
			else
			{
				if(lpChildren->getTexture() == null || lpChildren->m_lpVBData == null)
				{
					JGE2DQtree::getInstance()->getQtree()->clearObject(lpChildren);
				}
				else
				{
					JGE2DQtree::getInstance()->getQtree()->setObject(lpChildren, lpChildren->getBounds(&rect));
				}
			}
		}
	}
}