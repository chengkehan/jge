#include "jge.h"
#include "JGEButton.h"

using namespace jge;

JGEButton::JGEButton(IDirect3DDevice9* lpd3dd):JGEAbstractDisplayObject(lpd3dd)
{
	m_lpOutSkin = null;
	m_lpOverSkin = null;
	m_lpDownSkin = null;
	m_lpDisabledSkin = null;
	m_lpLabel = null;
}

JGEButton::~JGEButton()
{
	skinQtreeClear(m_lpOutSkin);
	skinQtreeClear(m_lpOverSkin);
	skinQtreeClear(m_lpDownSkin);
	skinQtreeClear(m_lpDisabledSkin);
	m_lpOutSkin = null;
	m_lpOverSkin = null;
	m_lpDownSkin = null;
	m_lpDisabledSkin = null;
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

void JGEButton::render()
{
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
	if(m_lpCurrentSkin != null)
	{
		m_lpCurrentSkin->qtreeSet();
	}
}

void JGEButton::qtreeClear()
{
	if(m_lpCurrentSkin != null)
	{
		m_lpCurrentSkin->qtreeClear();
	}
}

void JGEButton::qtreeSetClear()
{
	if(m_lpCurrentSkin != null)
	{
		m_lpCurrentSkin->qtreeSetClear();
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