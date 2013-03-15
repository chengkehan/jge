#include "jge.h"
#include "JGEButton.h"
#include "JGECallback.h"

using namespace jge;

JGEButton::JGEButton(IDirect3DDevice9* lpd3dd):JGEAbstractDisplayObject(lpd3dd)
{
	m_lpOutSkin = null;
	m_lpOverSkin = null;
	m_lpDownSkin = null;
	m_lpDisabledSkin = null;
	m_lpCurrentSkin = null;
	m_lpLabel = null;
	m_skinState = SkinState_Out;
	m_enabled = true;

	addEventListener<JGEButton>(JGEEvent::MOUSE_OVER, jgeCallbackThis(this, &JGEButton::mouseOverHandler));
	addEventListener<JGEButton>(JGEEvent::MOUSE_OUT, jgeCallbackThis(this, &JGEButton::mouseOutHandler));
	addEventListener<JGEButton>(JGEEvent::MOUSE_DOWN_LEFT, jgeCallbackThis(this, &JGEButton::mouseDownHandler));
	addEventListener<JGEButton>(JGEEvent::MOUSE_DOWN_RIGHT, jgeCallbackThis(this, &JGEButton::mouseDownHandler));
	addEventListener<JGEButton>(JGEEvent::MOUSE_DOWN_MIDDLE, jgeCallbackThis(this, &JGEButton::mouseDownHandler));
	addEventListener<JGEButton>(JGEEvent::MOUSE_UP_LEFT, jgeCallbackThis(this, &JGEButton::mouseUpHandler));
	addEventListener<JGEButton>(JGEEvent::MOUSE_UP_RIGHT, jgeCallbackThis(this, &JGEButton::mouseUpHandler));
	addEventListener<JGEButton>(JGEEvent::MOUSE_UP_MIDDLE, jgeCallbackThis(this, &JGEButton::mouseUpHandler));
}

JGEButton::~JGEButton()
{
	if(getParent() != null)
	{
		getParent()->removeChild(this);
	}

	m_lpOutSkin = null;
	m_lpOverSkin = null;
	m_lpDownSkin = null;
	m_lpDisabledSkin = null;
	m_lpCurrentSkin = null;
	releaseText(m_lpLabel);
}

void JGEButton::setLabel(const wchar_t* lpLabel)
{
	if(m_lpLabel == null)
	{
		m_lpLabel = newText();
		m_lpLabel->setTextFormat(DT_VCENTER | DT_CENTER | DT_NOCLIP);
		updateLabelBounds();
	}
	m_lpLabel->setText(lpLabel);
}

void JGEButton::updateLabelBounds()
{
	if(m_lpLabel == null)
	{
		return;
	}

	static RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (LONG)getWidth();
	rect.bottom = (LONG)getHeight();

	m_lpLabel->setTextBounds(&rect);
}

JGERect* JGEButton::getBoundsGlobal(JGERect* lpRectResult)
{
	if(lpRectResult == null || m_lpCurrentSkin == null)
	{
		return null;
	}
	else
	{
		return m_lpCurrentSkin->getBoundsGlobal(lpRectResult);
	}
}

bool JGEButton::inBoundsGlobal(float x, float y)
{
	if(m_lpCurrentSkin == null)
	{
		return false;
	}

	return m_lpCurrentSkin->inBoundsGlobal(x, y);
}

void JGEButton::render()
{
	if(m_enabled)
	{
		m_lpCurrentSkin = 
			m_skinState == SkinState_Down ? m_lpDownSkin : 
			m_skinState == SkinState_Up ? m_lpOverSkin : 
			m_skinState == SkinState_Out ? m_lpOutSkin : 
			m_skinState == SkinState_Over ? m_lpOverSkin : null;
	}
	else
	{
		m_lpCurrentSkin = m_lpDisabledSkin;
	}

	if(m_lpCurrentSkin != null)
	{
		m_lpCurrentSkin->render();
	}
}

bool JGEButton::shownInDisplayList()
{
	return getVisible() && ((m_lpLabel != null && m_lpLabel->shownInDisplayList()) || (m_lpCurrentSkin != null && m_lpCurrentSkin->shownInDisplayList()));
}

void JGEButton::qtreeSet()
{
	static JGERect rect;
	JGE2DQtree::getInstance()->getQtree()->setObject(this, getBoundsGlobal(&rect));
}

void JGEButton::qtreeClear()
{
	JGE2DQtree::getInstance()->getQtree()->clearObject(this);
}

void JGEButton::qtreeSetClear()
{
	if(shownInDisplayList())
	{
		qtreeSet();
	}
	else
	{
		qtreeClear();
	}
}

void JGEButton::updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent)
{
	if(lpMatrixGlobalParent == null)
	{
		return;
	}

	jgeMatrix2DRotationScalingTranslationDotProductAlpha(getRotation(), getScaleX(), getScaleY(), getX(), getY(), lpMatrixGlobalParent, getAlpha(), &m_matrixGlobal);

	if(m_lpCurrentSkin != null)
	{
		m_lpCurrentSkin->updateMatrixGlobal(&m_matrixGlobal);
	}
}

void JGEButton::mouseOverHandler(JGEEvent* lpEvent)
{
	m_skinState = SkinState_Over;
}

void JGEButton::mouseOutHandler(JGEEvent* lpEvent)
{
	m_skinState = SkinState_Out;
}

void JGEButton::mouseDownHandler(JGEEvent* lpEvent)
{
	m_skinState = SkinState_Down;
}

void JGEButton::mouseUpHandler(JGEEvent* lpEvent)
{
	m_skinState = SkinState_Up;
}