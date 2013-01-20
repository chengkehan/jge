#include "JGE2D.h"
#include "JGEInput.h"
#include "JGETextureManager.h"
#include "JGERender.h"
#include "JGEDisplayObject.h"

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
	int windowX, int windowY, uint windowWidth, uint windowHeight, bool windowd)
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

	if(!JGEInput::getInstance()->initInput(hInstance, JGE3D::getInstance()->getHWnd()))
	{
		return false;
	}
	if(!JGERender::getInstance()->init(JGE3D::getInstance()->getDirect3DDevice()))
	{
		return false;
	}

	jgeNewArgs1(m_lpStage, JGEDisplayObjectContainer, JGE3D::getInstance()->getDirect3DDevice());

	if(m_setupCallback != NULL)
	{
		if(!m_setupCallback())
		{
			return false;
		}
	}

	m_init = true;
	return true;
}

JGEDisplayObjectContainer* JGE2D::getStage() const
{
	return m_lpStage;
}

void JGE2D::setExitWhileEscapeDown(bool value)
{
	m_exitWhileEscapeDown = value;
}

bool JGE2D::getExitWhileEscapeDown() const
{
	return m_exitWhileEscapeDown;
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

bool JGE2D::getMouseVisible() const
{
	return m_mouseVisible;
}

void JGE2D::setMouseLockOnWindow(bool value)
{
	if(value)
	{
		JGE3D::getInstance()->setMessageCallback(WM_MOVE, jgeMouseLockOnWindowProc);
		jgeMouseLockOnWindowProc(NULL, 0, 0, 0);
	}
	else
	{
		JGE3D::getInstance()->clearMessageCallback(WM_MOVE);
		JGEInput::getInstance()->mouseUnlockOnWindow();
	}
}

bool JGE2D::getMouseLockOnWindow() const
{
	return JGEInput::getInstance()->getMouseLockedOnWindow();
}

void JGE2D::jgeFrameCallback(uint timeDelta)
{
	JGEInput::getInstance()->updateInput();
	if(JGE2D::getInstance()->m_exitWhileEscapeDown && JGEInput::getInstance()->keyDown(DIK_ESCAPE))
	{
		jgeWin32Exit();
		return;
	}

	//jgeUpdateMouseEvent(JGE2D::getInstance()->getStage());

	if(JGE2D::getInstance()->m_frameCallback != NULL)
	{
		JGE2D::getInstance()->m_frameCallback(timeDelta);
	}

	JGERender::getInstance()->beginScene();
	jgeRenderDisplayObjectContainer(JGE2D::getInstance()->getStage());
	JGERender::getInstance()->endScene();
}

void JGE2D::jgeRenderDisplayObjectContainer(JGEDisplayObjectContainer* lpContainer)
{
	if(lpContainer == NULL)
	{
		return;
	}
	else
	{
		JGEDisplayObjectContainer::ChildrenList* lpChildren = &lpContainer->m_lpChildrenList;
		for(JGEDisplayObjectContainer::ChildrenList::iterator iter = lpChildren->begin(); iter != lpChildren->end(); ++iter)
		{
			JGEDisplayObject* lpChild = *iter;
			if(lpChild->m_isContainer)
			{
				jgeRenderDisplayObjectContainer((JGEDisplayObjectContainer*)lpChild);
			}
			else
			{
				lpChild->updateVertexBufferData();
				JGERender::getInstance()->renderDisplayObject(lpChild);
			}
		}
	}
}

void JGE2D::jgeMouseLockOnWindowProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam)
{
	JGEInput::getInstance()->mouseLockOnWindow();
}

//void JGE2D::jgeUpdateMouseEvent(JCDisplayObjectContainer* lpContainer)
//{
//	//jccommon_stdRIterForEachM(std::list<JCDisplayObject*>, lpContainer->m_childrenList, iter)
//	//{
//	//	JCDisplayObject* child = *iter;
//	//	if(child->isContainer())
//	//	{
//	//		jc2dUpdateMouseEvent((JCDisplayObjectContainer*)child);
//	//	}
//	//	else
//	//	{
//
//	//	}
//	//}
//}