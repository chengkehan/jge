#include "JGEText.h"
#include "JGEDisplayObjectContainer.h"
#include "JGERender.h"

JGEPoint JGEText::m_boundsTemp[4];

JGEText::JGEText(IDirect3DDevice9* lpd3dd):JGEAbstractDisplayObject(lpd3dd)
{
	m_lpSprite = null;
	m_lpFont = null;
	m_lpStr = null;
	m_color = 0xFFFFFF;
	m_rect.left = 0; m_rect.top = 0; m_rect.right = 1; m_rect.bottom = 1;
	m_dt_foramt = DT_LEFT | DT_TOP | DT_NOCLIP;
	setConfig(12, 25, 500, false, L"DefaultFont");
	updateColor();
}

JGEText::~JGEText()
{
	if(getParent() != null)
	{
		getParent()->removeChild(this);
	}

	jgeReleaseCom(m_lpFont);
	jgewcsfree(m_lpStr);
	jgeReleaseCom(m_lpSprite);
}

void JGEText::setText(const wchar_t* lpStr)
{
	if(jgewcsequ(lpStr, m_lpStr))
	{
		return;
	}

	jgewcsfree(m_lpStr);
	m_lpStr = jgewcsclone(lpStr);
}

const wchar_t* JGEText::getText() const
{
	return m_lpStr;
}

void JGEText::setTextColor(uint color)
{
	m_color = color;
	updateColor();
}

uint JGEText::getTextColor() const
{
	return m_color;
}

void JGEText::setTextBounds(const RECT* lpRect)
{
	if(lpRect == null)
	{
		return;
	}

	m_rect.left = lpRect->left;
	m_rect.top = lpRect->top;
	m_rect.right = lpRect->right;
	m_rect.bottom = lpRect->bottom;
}

const RECT* JGEText::getTextBounds() const
{
	return &m_rect;
}

void JGEText::setTextFormat(uint dt_format)
{
	m_dt_foramt = dt_format;
}

uint JGEText::getTextFormat() const
{
	return m_dt_foramt;
}

void JGEText::setConfig(int width, int height, int weight, bool italic, const wchar_t* lpFaceName)
{
	if(width != -1)
	{
		m_desc.Width = width;
	}
	if(height != -1)
	{
		m_desc.Height = height;
	}
	if(weight != -1)
	{
		m_desc.Weight = weight;
	}
	m_desc.MipLevels = D3DX_DEFAULT; 
	m_desc.Italic = italic; 
	m_desc.CharSet = DEFAULT_CHARSET; 
	m_desc.OutputPrecision = 0; m_desc.Quality = 0;
	if(lpFaceName != null)
	{
		jgewcsclone(lpFaceName, m_desc.FaceName);
	}
	resetFont();
}

const D3DXFONT_DESCW* JGEText::getConfig() const
{
	return &m_desc;
}

JGERect* JGEText::getBoundsGlobal(JGERect* lpRectResult)
{
	if(lpRectResult == null || m_lpSprite == null || jgewcslen(m_lpStr) == 0)
	{
		return null;
	}
	else
	{
		static RECT rect;
		static JGEVector2D vect;

		m_lpFont->DrawText(null, m_lpStr, -1, &rect, m_dt_foramt | DT_CALCRECT, m_textColor);
		
		vect.m_x = (float)rect.left;
		vect.m_y = (float)rect.top;
		jgeVector2DTransform(&vect, &m_matrixGlobal);
		m_boundsTemp[0].m_x = vect.m_x;
		m_boundsTemp[0].m_y = vect.m_y;

		vect.m_x = (float)rect.right;
		vect.m_y = (float)rect.top;
		jgeVector2DTransform(&vect, &m_matrixGlobal);
		m_boundsTemp[1].m_x = vect.m_x;
		m_boundsTemp[1].m_y = vect.m_y;

		vect.m_x = (float)rect.right;
		vect.m_y = (float)rect.bottom;
		jgeVector2DTransform(&vect, &m_matrixGlobal);
		m_boundsTemp[2].m_x = vect.m_x;
		m_boundsTemp[2].m_y = vect.m_y;

		vect.m_x = (float)rect.left;
		vect.m_y = (float)rect.bottom;
		jgeVector2DTransform(&vect, &m_matrixGlobal);
		m_boundsTemp[3].m_x = vect.m_x;
		m_boundsTemp[3].m_y = vect.m_y;

		lpRectResult->m_left = min(min(min(m_boundsTemp[0].m_x, m_boundsTemp[1].m_x), m_boundsTemp[2].m_x), m_boundsTemp[3].m_x);
		lpRectResult->m_top = min(min(min(m_boundsTemp[0].m_y, m_boundsTemp[1].m_y), m_boundsTemp[2].m_y), m_boundsTemp[3].m_y);
		lpRectResult->m_right = max(max(max(m_boundsTemp[0].m_x, m_boundsTemp[1].m_x), m_boundsTemp[2].m_x), m_boundsTemp[3].m_x);
		lpRectResult->m_bottom = max(max(max(m_boundsTemp[0].m_y, m_boundsTemp[1].m_y), m_boundsTemp[2].m_y), m_boundsTemp[3].m_y);

		return lpRectResult;
	}
}

bool JGEText::inBoundsGlobal(float x, float y)
{
	static JGERect rect;
	static JGEPoint point;

	if(getBoundsGlobal(&rect) == null)
	{
		return false;
	}
	if(!rect.contains(x, y))
	{
		return false;
	}
	point.m_x = x; point.m_y = y;
	if(
		(jgeVectorABPointSide(&m_boundsTemp[1], &m_boundsTemp[0], &point) != -1 || 
		jgeVectorABPointSide(&m_boundsTemp[2], &m_boundsTemp[1], &point) != -1 || 
		jgeVectorABPointSide(&m_boundsTemp[0], &m_boundsTemp[2], &point) != -1)
		&& 
		(jgeVectorABPointSide(&m_boundsTemp[2], &m_boundsTemp[0], &point) != -1 || 
		jgeVectorABPointSide(&m_boundsTemp[3], &m_boundsTemp[2], &point) != -1 || 
		jgeVectorABPointSide(&m_boundsTemp[0], &m_boundsTemp[3], &point) != -1)
		)
	{
		return false;
	}

	return true;
}

void JGEText::resetFont()
{
	jgeReleaseCom(m_lpFont);
	jgeDXVerifyIf(D3DXCreateFontIndirect(getDirect3DDevice(), &m_desc, &m_lpFont))jgeDXVerifyEndIf
}

void JGEText::render()
{
	if(!shownInDisplayList())
	{
		return;
	}

	if(m_lpSprite == null)
	{
		jgeDXVerifyIf(D3DXCreateSprite(getDirect3DDevice(), &m_lpSprite))jgeDXVerifyEndIf
	}

	JGERender::getInstance()->endScene();

	static D3DXMATRIX matrixD3D;
	jgeMatrix2DToD3DXMatrix(&m_matrixGlobal, &matrixD3D);
	matrixD3D._14 = 0.0f;
	m_lpSprite->SetTransform(&matrixD3D);
	m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_lpFont->DrawText(m_lpSprite, m_lpStr, -1, &m_rect, m_dt_foramt, m_textColor);
	m_lpSprite->End();

	JGERender::getInstance()->beginScene();
}

bool JGEText::shownInDisplayList()
{
	return getVisible() && jgewcslen(m_lpStr) > 0;
}

void JGEText::qtreeSet()
{
	static JGERect rect;
	JGE2DQtree::getInstance()->getQtree()->setObject(this, getBoundsGlobal(&rect));
}

void JGEText::qtreeClear()
{
	JGE2DQtree::getInstance()->getQtree()->clearObject(this);
}

void JGEText::qtreeSetClear()
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

void JGEText::updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent)
{
	if(lpMatrixGlobalParent == null)
	{
		return;
	}

	jgeMatrix2DRotationScalingTranslationDotProductAlpha(getRotation(), getScaleX(), getScaleY(), getX(), getY(), lpMatrixGlobalParent, getAlpha(), &m_matrixGlobal);
}