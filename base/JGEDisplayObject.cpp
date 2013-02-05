#include "JGEDisplayObject.h"
#include "JGEDisplayObjectContainer.h"

const DWORD JGEDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

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
	if(m_lpParent != null)
	{
		m_lpParent->removeChild(this);
	}
	m_lpd3dd = null;
	m_lpTexture = null;
	m_lpParent = null;
	jgeDelete(m_lpVBData);
	JGE2DQtree::getInstance()->getQtree()->clearObject(this);
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

JGEPoint* JGEDisplayObject::getBounds(JGEPoint* lpBoundsResult)
{
	if(lpBoundsResult == null || m_lpVBData == null || m_lpTexture == null)
	{
		return null;
	}
	else
	{
		lpBoundsResult[0].m_x = m_lpVBData[0].x;
		lpBoundsResult[0].m_y = m_lpVBData[0].y;

		lpBoundsResult[1].m_x = m_lpVBData[1].x;
		lpBoundsResult[1].m_y = m_lpVBData[1].y;

		lpBoundsResult[2].m_x = m_lpVBData[3].x;
		lpBoundsResult[2].m_y = m_lpVBData[3].y;

		lpBoundsResult[3].m_x = m_lpVBData[2].x;
		lpBoundsResult[3].m_y = m_lpVBData[2].y;

		return lpBoundsResult;
	}
}

bool JGEDisplayObject::inBounds(JGEPoint* lpBounds, JGEPoint* lpPoint)
{
	if(lpBounds == null || lpPoint == null)
	{
		return false;
	}

	if(
		(jgeVectorABPointSide(&lpBounds[1], &lpBounds[0], lpPoint) != -1 || 
		jgeVectorABPointSide(&lpBounds[2], &lpBounds[1], lpPoint) != -1 || 
		jgeVectorABPointSide(&lpBounds[0], &lpBounds[2], lpPoint) != -1)
		&& 
		(jgeVectorABPointSide(&lpBounds[2], &lpBounds[0], lpPoint) != -1 || 
		jgeVectorABPointSide(&lpBounds[3], &lpBounds[2], lpPoint) != -1 || 
		jgeVectorABPointSide(&lpBounds[0], &lpBounds[3], lpPoint) != -1)
	)
	{
		return false;
	}

	return true;
}

bool JGEDisplayObject::inBounds(JGEPoint* lpBounds, float pointX, float pointY)
{
	static JGEPoint point;
	point.m_x = pointX;
	point.m_y = pointY;
	return inBounds(lpBounds, &point);
}

void JGEDisplayObject::updateVertexBufferData()
{
	// x1=cos(angle)*x-sin(angle)*y;
	// y1=cos(angle)*y+sin(angle)*x;

	if(m_lpVBData == null)
	{
		jgeNewArray(m_lpVBData, JGEDisplayObject::Vertex, 4 * sizeof(JGEDisplayObject::Vertex));
		m_lpVBData[0].u = 0.0f; m_lpVBData[0].v = 1.0f; m_lpVBData[0].diffuse = 0xFF000000; m_lpVBData[0].x = 0.0f; m_lpVBData[0].y = 0.0f; m_lpVBData[0].rhw = 1.0f; m_lpVBData[0].z = 0.0f;
		m_lpVBData[1].u = 0.0f; m_lpVBData[1].v = 0.0f; m_lpVBData[1].diffuse = 0xFF000000; m_lpVBData[1].x = 0.0f; m_lpVBData[1].y = 0.0f; m_lpVBData[1].rhw = 1.0f; m_lpVBData[1].z = 0.0f;
		m_lpVBData[2].u = 1.0f; m_lpVBData[2].v = 1.0f; m_lpVBData[2].diffuse = 0xFF000000; m_lpVBData[2].x = 0.0f; m_lpVBData[2].y = 0.0f; m_lpVBData[2].rhw = 1.0f; m_lpVBData[2].z = 0.0f;
		m_lpVBData[3].u = 1.0f; m_lpVBData[3].v = 0.0f; m_lpVBData[3].diffuse = 0xFF000000; m_lpVBData[3].x = 0.0f; m_lpVBData[3].y = 0.0f; m_lpVBData[3].rhw = 1.0f; m_lpVBData[3].z = 0.0f;
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