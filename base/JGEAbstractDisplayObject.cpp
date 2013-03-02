#include "JGEAbstractDisplayObject.h"
#include "JGEDisplayObjectContainer.h"

JGEAbstractDisplayObject::JGEAbstractDisplayObject(IDirect3DDevice9* lpd3dd)
{
	jgeAssert(lpd3dd != null);
	m_lpd3dd = lpd3dd;
	m_x = 0.0f; m_y = 0.0f;
	m_widthOriginal = 0.0f; m_heightOriginal = 0.0f;
	m_scaleX = 1.0f; m_scaleY = 1.0f;
	m_rotation = 0.0f;
	m_alpha = 1.0f;
	m_lpParent = null;
}

JGEAbstractDisplayObject::~JGEAbstractDisplayObject()
{
	m_lpd3dd = null;
	m_lpParent = null;
	JGE2DQtree::getInstance()->getQtree()->clearObject(this);
}