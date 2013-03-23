#include "JGEDisplayObject.h"
#include "JGEDisplayObjectContainer.h"
#include "JGERender.h"

const DWORD JGEDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

JGEDisplayObject::JGEDisplayObject(IDirect3DDevice9* lpd3dd):JGEAbstractDisplayObject(lpd3dd)
{
	m_refX = 0.0f; m_refY = 0.0f;
	m_lpTexture = null;
	m_lpVBData = null;
	m_alphaEnabled = true;
	m_numRenderRect = 1;
}

JGEDisplayObject::~JGEDisplayObject()
{
	if(getParent() != null)
	{
		getParent()->removeChild(this);
	}

	m_lpTexture = null;
	jgeDelete(m_lpVBData);
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

JGERect* JGEDisplayObject::getBoundsGlobal(JGERect* lpRectResult)
{
	if(lpRectResult == null)
	{
		return null;
	}

	if(m_lpVBData == null || m_lpTexture == null)
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

bool JGEDisplayObject::inBoundsGlobal(float x, float y)
{
	static JGERect bounds;
	static JGEPoint bounds2[4];
	static JGEPoint point;
	if(getBoundsGlobal(&bounds) == null)
	{
		return false;
	}
	if(!bounds.contains(x, y))
	{
		return false;
	}
	if(getBoundsVectorGlobal(bounds2) == null)
	{
		return false;
	}
	point.m_x = x; point.m_y = y;
	if(
		(jgeVectorABPointSide(&bounds2[1], &bounds2[0], &point) != -1 || 
		jgeVectorABPointSide(&bounds2[2], &bounds2[1], &point) != -1 || 
		jgeVectorABPointSide(&bounds2[0], &bounds2[2], &point) != -1)
		&& 
		(jgeVectorABPointSide(&bounds2[2], &bounds2[0], &point) != -1 || 
		jgeVectorABPointSide(&bounds2[3], &bounds2[2], &point) != -1 || 
		jgeVectorABPointSide(&bounds2[0], &bounds2[3], &point) != -1)
		)
	{
		return false;
	}

	return true;
}

void JGEDisplayObject::render()
{
	if(!shownInDisplayList())
	{
		return;
	}

	// create vb
	if(m_lpVBData == null)
	{
		jgeNewArray(m_lpVBData, JGEDisplayObject::Vertex, 4 * sizeof(JGEDisplayObject::Vertex) * m_numRenderRect);
		m_lpVBData[0].u = 0.0f; m_lpVBData[0].v = 1.0f; m_lpVBData[0].diffuse = 0xFF000000; m_lpVBData[0].x = 0.0f; m_lpVBData[0].y = 0.0f; m_lpVBData[0].rhw = 1.0f; m_lpVBData[0].z = 0.0f;
		m_lpVBData[1].u = 0.0f; m_lpVBData[1].v = 0.0f; m_lpVBData[1].diffuse = 0xFF000000; m_lpVBData[1].x = 0.0f; m_lpVBData[1].y = 0.0f; m_lpVBData[1].rhw = 1.0f; m_lpVBData[1].z = 0.0f;
		m_lpVBData[2].u = 1.0f; m_lpVBData[2].v = 1.0f; m_lpVBData[2].diffuse = 0xFF000000; m_lpVBData[2].x = 0.0f; m_lpVBData[2].y = 0.0f; m_lpVBData[2].rhw = 1.0f; m_lpVBData[2].z = 0.0f;
		m_lpVBData[3].u = 1.0f; m_lpVBData[3].v = 0.0f; m_lpVBData[3].diffuse = 0xFF000000; m_lpVBData[3].x = 0.0f; m_lpVBData[3].y = 0.0f; m_lpVBData[3].rhw = 1.0f; m_lpVBData[3].z = 0.0f;
	}

	// update uv
	float u = (float)m_lpTexture->getImageInfo()->Width / (float)m_lpTexture->getSurfaceDesc()->Width;
	float v = (float)m_lpTexture->getImageInfo()->Height / (float)m_lpTexture->getSurfaceDesc()->Height;
	m_lpVBData[0].u = 0.0f; m_lpVBData[0].v = v;
	m_lpVBData[1].u = 0.0f; m_lpVBData[1].v = 0.0f;
	m_lpVBData[2].u = u; m_lpVBData[2].v = v;
	m_lpVBData[3].u = u; m_lpVBData[3].v = 0.0f;

	// update xy
	static JGEVector2D point;

	point.m_x = -m_refX;
	point.m_y = m_heightOriginal - m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[0].x = point.m_x;
	m_lpVBData[0].y = point.m_y;

	point.m_x = -m_refX;
	point.m_y = -m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[1].x = point.m_x;
	m_lpVBData[1].y = point.m_y;

	point.m_x = m_widthOriginal - m_refX;
	point.m_y = m_heightOriginal - m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[2].x = point.m_x;
	m_lpVBData[2].y = point.m_y;

	point.m_x = m_widthOriginal - m_refX;
	point.m_y = -m_refY;
	jgeVector2DTransform(&point, &m_matrixGlobal);
	m_lpVBData[3].x = point.m_x;
	m_lpVBData[3].y = point.m_y;

	// update color & alpha
	m_lpVBData[0].diffuse = (((int)(m_matrixGlobal.m_13 * 255.0f) & 0xFF) << 24) + (m_lpVBData[0].diffuse & 0xFFFFFF);
	m_lpVBData[1].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[2].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[3].diffuse = m_lpVBData[0].diffuse;

	// render
	JGERender::getInstance()->renderDisplayObject(this);
}

bool JGEDisplayObject::shownInDisplayList()
{
	return getVisible() && m_lpTexture != null;
}

void JGEDisplayObject::qtreeSet()
{
	static JGERect rect;
	JGE2DQtree::getInstance()->getQtree()->setObject(this, getBoundsGlobal(&rect));
}

void JGEDisplayObject::qtreeClear()
{
	JGE2DQtree::getInstance()->getQtree()->clearObject(this);
}

void JGEDisplayObject::qtreeSetClear()
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

void JGEDisplayObject::updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent)
{
	if(lpMatrixGlobalParent == null)
	{
		return;
	}

	jgeMatrix2DRotationScalingTranslationDotProductAlpha(getRotation(), getScaleX(), getScaleY(), getX(), getY(), lpMatrixGlobalParent, getAlpha(), &m_matrixGlobal);
}

JGEPoint* JGEDisplayObject::getBoundsVectorGlobal(JGEPoint* lpBoundsResult)
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