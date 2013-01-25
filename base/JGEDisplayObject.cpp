#include "JGEDisplayObject.h"
#include "JGEDisplayObjectContainer.h"

CONST DWORD JGEDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

JGEDisplayObject::JGEDisplayObject(IDirect3DDevice9* lpd3dd)
{
	jgeAssert(lpd3dd != null);
	m_lpd3dd = lpd3dd;
	m_refX = 0.0f; m_refY = 0.0f;
	m_x = 0.0f; m_y = 0.0f;
	m_widthOriginal = 0.0f; m_heightOriginal = 0.0f;
	m_scaleX = 1.0f; m_scaleY = 1.0f;
	m_rotation = 0.0f;
	m_lpTexture = null;
	m_lpVBData = null;
	m_alpha = 1.0f; m_alphaEnabled = true;
	m_lpParent = null;
	m_isContainer = false;
}

JGEDisplayObject::~JGEDisplayObject()
{
	m_lpd3dd = null;
	m_lpTexture = null;
	m_lpParent = null;
	jgeDelete(m_lpVBData);
	JGE2DQtree::getInstance()->getQtree()->clearObject(this);
}

inline void JGEDisplayObject::setX(float value)
{
	m_x = value;
}

inline float JGEDisplayObject::getX() const
{
	return m_x;
}

inline void JGEDisplayObject::setY(float value)
{
	m_y = value;
}

inline float JGEDisplayObject::getY() const
{
	return m_y;
}

inline void JGEDisplayObject::setRefX(float value)
{
	m_refX = value;
}

inline float JGEDisplayObject::getRefX() const
{
	return m_refX;
}

inline void JGEDisplayObject::setRefY(float value)
{
	m_refY = value;
}

inline float JGEDisplayObject::getRefY() const
{
	return m_refY;
}

inline void JGEDisplayObject::setWidth(float value)
{
	m_scaleX = value / m_widthOriginal;
}

inline float JGEDisplayObject::getWidth() const
{
	return m_widthOriginal * m_scaleX;
}

inline float JGEDisplayObject::getWidthOriginal() const
{
	return m_widthOriginal;
}

inline void JGEDisplayObject::setHeight(float value)
{
	m_scaleY = value / m_heightOriginal;
}

inline float JGEDisplayObject::getHeight() const
{
	return m_heightOriginal * m_scaleY;
}

inline float JGEDisplayObject::getHeightOriginal() const
{
	return m_heightOriginal;
}

void JGEDisplayObject::setScaleX(float value)
{
	m_scaleX = value;
}

inline float JGEDisplayObject::getScaleX() const
{
	return m_scaleX;
}

inline void JGEDisplayObject::setScaleY(float value)
{
	m_scaleY = value;
}

float JGEDisplayObject::getScaleY() const
{
	return m_scaleY;
}

inline void JGEDisplayObject::setRotation(float value)
{
	m_rotation = value;
}

inline float JGEDisplayObject::getRotation() const
{
	return m_rotation;
}

bool JGEDisplayObject::setTexture(JGETexture* lpTexture)
{
	m_lpTexture = lpTexture;
	if(m_lpTexture == null)
	{
		m_widthOriginal = 0.0f;
		m_heightOriginal = 0.0f;
	}
	else
	{
		m_widthOriginal = (float)lpTexture->getImageInfo()->Width;
		m_heightOriginal = (float)lpTexture->getImageInfo()->Height;
	}
	return true;
}

inline JGETexture* JGEDisplayObject::getTexture() const
{
	return m_lpTexture;
}

inline void JGEDisplayObject::setAlpha(float value)
{
	m_alpha = min(max(value, 0.0f), 1.0f);
}

inline float JGEDisplayObject::getAlpha() const
{
	return m_alpha;
}

inline void JGEDisplayObject::setAlphaEnabled(bool value)
{
	m_alphaEnabled = value;
}

inline bool JGEDisplayObject::getAlphaEnabled() const
{
	return m_alphaEnabled;
}

inline JGEDisplayObjectContainer* JGEDisplayObject::getParent() const
{
	return m_lpParent;
}

JGERect* JGEDisplayObject::getBounds(JGERect* lpRectResult)
{
	if(lpRectResult == null)
	{
		return null;
	}

	if(m_lpVBData == null)
	{
		return null;
	}
	else
	{
		lpRectResult->m_left = min(min(min(m_lpVBData[0].x, m_lpVBData[1].x), m_lpVBData[2].x), m_lpVBData[3].x);
		lpRectResult->m_top = min(min(min(m_lpVBData[0].y, m_lpVBData[1].y), m_lpVBData[2].y), m_lpVBData[3].y);
		lpRectResult->m_right = max(max(max(m_lpVBData[0].x, m_lpVBData[1].x), m_lpVBData[2].x), m_lpVBData[3].x);
		lpRectResult->m_bottom = max(max(max(m_lpVBData[0].y, m_lpVBData[1].y), m_lpVBData[2].y), m_lpVBData[3].y);
		return lpRectResult;
	}
}

inline void JGEDisplayObject::setParent(JGEDisplayObjectContainer* lpParent)
{
	m_lpParent = lpParent;
}

void JGEDisplayObject::updateVertexBufferData()
{
	// x1=cos(angle)*x-sin(angle)*y;
	// y1=cos(angle)*y+sin(angle)*x;

	if(m_lpVBData == null)
	{
		jgeNewArray(m_lpVBData, JGEDisplayObject::Vertex, 4 * sizeof(JGEDisplayObject::Vertex));
		m_lpVBData[0].u = 0.0f; m_lpVBData[0].v = 1.0f; m_lpVBData[0].diffuse = 0xFF000000; m_lpVBData[0].x = 0.0f; m_lpVBData[0].y = 0.0f; m_lpVBData[0].rhw = 0.0f; m_lpVBData[0].z = 0.0f;
		m_lpVBData[1].u = 0.0f; m_lpVBData[1].v = 0.0f; m_lpVBData[1].diffuse = 0xFF000000; m_lpVBData[1].x = 0.0f; m_lpVBData[1].y = 0.0f; m_lpVBData[1].rhw = 0.0f; m_lpVBData[1].z = 0.0f;
		m_lpVBData[2].u = 1.0f; m_lpVBData[2].v = 1.0f; m_lpVBData[2].diffuse = 0xFF000000; m_lpVBData[2].x = 0.0f; m_lpVBData[2].y = 0.0f; m_lpVBData[2].rhw = 0.0f; m_lpVBData[2].z = 0.0f;
		m_lpVBData[3].u = 1.0f; m_lpVBData[3].v = 0.0f; m_lpVBData[3].diffuse = 0xFF000000; m_lpVBData[3].x = 0.0f; m_lpVBData[3].y = 0.0f; m_lpVBData[3].rhw = 0.0f; m_lpVBData[3].z = 0.0f;
	}

	float global_x = 0.0f;
	float global_y = 0.0f;
	float global_scaleX = 1.0f;
	float global_scaleY = 1.0f;
	float global_rotation = 0.0f;
	float global_alpha = 1.0f;
	JGEDisplayObjectContainer * lpTarget = m_lpParent;
	while(lpTarget != null)
	{
		global_x += lpTarget->getX() + lpTarget->getRefX();
		global_y += lpTarget->getY() + lpTarget->getRefY();
		global_scaleX *= lpTarget->getScaleX();
		global_scaleY *= lpTarget->getScaleY();
		global_rotation += lpTarget->getRotation();
		global_alpha *= lpTarget->getAlpha();
		lpTarget = lpTarget->getParent();
	}

	float x = global_x + m_x;
	float y = global_y + m_y;
	float width = m_widthOriginal * m_scaleX * global_scaleX;
	float height = m_heightOriginal * m_scaleY * global_scaleY;
	float rotation = m_rotation + global_rotation;
	float refX = m_refX * m_scaleX * global_scaleX;
	float refY = m_refY * m_scaleY * global_scaleY;

	float cosRot = cosf(rotation);
	float sinRot = sinf(rotation);

	m_lpVBData[0].x = x + cosRot * (-refX) - sinRot * (height - refY);
	m_lpVBData[0].y = y + cosRot * (height - refY) + sinRot * (-refX);

	m_lpVBData[1].x = x + cosRot * (-refX) - sinRot * (-refY);
	m_lpVBData[1].y = y + cosRot * (-refY) + sinRot * (-refX);

	m_lpVBData[2].x = x + cosRot * (width - refX) - sinRot * (height - refY);
	m_lpVBData[2].y = y + cosRot * (height - refY) + sinRot * (width - refX);

	m_lpVBData[3].x = x + cosRot * (width - refX) - sinRot * (-refY);
	m_lpVBData[3].y = y + cosRot * (-refY) + sinRot * (width - refX);

	m_lpVBData[0].diffuse = (((int)(global_alpha * m_alpha * 255.0f) & 0xFF) << 24) + (m_lpVBData[0].diffuse & 0xFFFFFF);
	m_lpVBData[1].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[2].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[3].diffuse = m_lpVBData[0].diffuse;
}