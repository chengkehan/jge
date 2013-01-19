#include "JGEDisplayObject.h"
#include "JGEDisplayObjectContainer.h"
#include "jgeUtil.h"

CONST DWORD JGEDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

JGEDisplayObject::JGEDisplayObject(IDirect3DDevice9* lpd3dd)
{
	jgeAssert(lpd3dd != null);
	m_lpd3dd = lpd3dd;
	m_refX = 0.0f; m_refY = 0.0f;
	m_x = 0.0f; m_y = 0.0f;
	m_widthOriginal = 0.0f; m_heightOriginal = 0.0f;
	m_scaleX = 0.0f; m_scaleY = 0.0f;
	m_rotation = 0.0f;
	m_lpTexture = null;
	m_lpVBData = null;
	m_alpha = 0.0f; m_alphaEnabled = true;
	m_lpParent = null;
}

JGEDisplayObject::~JGEDisplayObject()
{
	m_lpd3dd = null;
	m_lpTexture = null;
	m_lpParent = null;
	jgeDelete(m_lpVBData);
}

void JGEDisplayObject::setX(float value)
{
	m_x = value;
}

float JGEDisplayObject::getX() const
{
	return m_x;
}

void JGEDisplayObject::setY(float value)
{
	m_y = value;
}

float JGEDisplayObject::getY() const
{
	return m_y;
}

void JGEDisplayObject::setRefX(float value)
{
	m_refX = value;
}

float JGEDisplayObject::getRefX() const
{
	return m_refX;
}

void JGEDisplayObject::setRefY(float value)
{
	m_refY = value;
}

float JGEDisplayObject::getRefY() const
{
	return m_refY;
}

void JGEDisplayObject::setWidth(float value)
{
	m_scaleX = value / m_widthOriginal;
}

float JGEDisplayObject::getWidth() const
{
	return m_widthOriginal * m_scaleX;
}

float JGEDisplayObject::getWidthOriginal() const
{
	return m_widthOriginal;
}

void JGEDisplayObject::setHeight(float value)
{
	m_scaleY = value / m_heightOriginal;
}

float JGEDisplayObject::getHeight() const
{
	return m_heightOriginal * m_scaleY;
}

float JGEDisplayObject::getHeightOriginal() const
{
	return m_heightOriginal;
}

void JGEDisplayObject::setScaleX(float value)
{
	m_scaleX = value;
}

float JGEDisplayObject::getScaleX() const
{
	return m_scaleX;
}

void JGEDisplayObject::setScaleY(float value)
{
	m_scaleY = value;
}

float JGEDisplayObject::getScaleY() const
{
	return m_scaleY;
}

void JGEDisplayObject::setRotation(float value)
{
	m_rotation = value;
}

float JGEDisplayObject::getRotation() const
{
	return m_rotation;
}

void JGEDisplayObject::setTexture(JGETexture* lpTexture)
{
	m_lpTexture = lpTexture;
	if(m_lpTexture != NULL)
	{
		m_widthOriginal = 0.0f;
		m_heightOriginal = 0.0f;
	}
	else
	{
		m_widthOriginal = (float)lpTexture->getImageInfo()->Width;
		m_heightOriginal = (float)lpTexture->getImageInfo()->Height;
	}
}

JGETexture* JGEDisplayObject::getTexture() const
{
	return m_lpTexture;
}

void JGEDisplayObject::setAlpha(float value)
{
	m_alpha = min(max(value, 0.0f), 1.0f);
}

float JGEDisplayObject::getAlpha() const
{
	return m_alpha;
}

void JGEDisplayObject::setAlphaEnabled(bool value)
{
	m_alphaEnabled = value;
}

bool JGEDisplayObject::getAlphaEnabled() const
{
	return m_alphaEnabled;
}

JGEDisplayObjectContainer* JGEDisplayObject::getParent() const
{
	return m_lpParent;
}

void JGEDisplayObject::setParent(JGEDisplayObjectContainer* lpParent)
{
	m_lpParent = lpParent;
}

void JGEDisplayObject::updateVertexBufferData()
{
	// x1=cos(angle)*x-sin(angle)*y;
	// y1=cos(angle)*y+sin(angle)*x;

	FLOAT global_x = 0.0f;
	FLOAT global_y = 0.0f;
	FLOAT global_scaleX = 1.0f;
	FLOAT global_scaleY = 1.0f;
	FLOAT global_rotation = 0.0f;
	FLOAT global_alpha = 1.0f;
	JGEDisplayObjectContainer * lpTarget = m_lpParent;
	while(lpTarget != NULL)
	{
		global_x += lpTarget->getX() + lpTarget->getRefX();
		global_y += lpTarget->getY() + lpTarget->getRefY();
		global_scaleX *= lpTarget->getScaleX();
		global_scaleY *= lpTarget->getScaleY();
		global_rotation += lpTarget->getRotation();
		global_alpha *= lpTarget->getAlpha();
		lpTarget = lpTarget->getParent();
	}

	FLOAT x = global_x + m_x;
	FLOAT y = global_y + m_y;
	FLOAT width = m_widthOriginal * m_scaleX * global_scaleX;
	FLOAT height = m_heightOriginal * m_scaleY * global_scaleY;
	FLOAT rotation = m_rotation + global_rotation;
	FLOAT refX = m_refX * m_scaleX * global_scaleX;
	FLOAT refY = m_refY * m_scaleY * global_scaleY;

	m_lpVBData[0].x = x + cosf(rotation) * (-refX) - sinf(rotation) * (height - refY);
	m_lpVBData[0].y = y + cosf(rotation) * (height - refY) + sinf(rotation) * (-refX);

	m_lpVBData[1].x = x + cosf(rotation) * (-refX) - sinf(rotation) * (-refY);
	m_lpVBData[1].y = y + cosf(rotation) * (-refY) + sinf(rotation) * (-refX);

	m_lpVBData[2].x = x + cosf(rotation) * (width - refX) - sinf(rotation) * (height - refY);
	m_lpVBData[2].y = y + cosf(rotation) * (height - refY) + sinf(rotation) * (width - refX);

	m_lpVBData[3].x = x + cosf(rotation) * (width - refX) - sinf(rotation) * (-refY);
	m_lpVBData[3].y = y + cosf(rotation) * (-refY) + sinf(rotation) * (width - refX);

	m_lpVBData[0].diffuse = (((INT)(global_alpha * m_alpha * 255.0f) & 0xFF) << 24) + (m_lpVBData[0].diffuse & 0xFFFFFF);
	m_lpVBData[1].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[2].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[3].diffuse = m_lpVBData[0].diffuse;
}

inline void JGEDisplayObject::initVBData()
{
	if(m_lpVBData == null)
	{
		jgeNewArray(m_lpVBData, JGEDisplayObject::Vertex, 4 * sizeof(JGEDisplayObject::Vertex));
	}
}