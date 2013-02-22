#include "JGEText.h"
#include "JGEDisplayObjectType.h"
#include "JGEDisplayObjectContainer.h"

JGEText::JGEText(IDirect3DDevice9* lpd3dd):JGEDisplayObject(lpd3dd)
{
	m_displayObjectType = JGE_DISPLAYOBJECT_TEXT_TYPE;
	m_lpSprite = null;
	m_lpFont = null;
	m_lpStr = null;
	m_textColor = 0xFFFFFFFF;
	m_rect.left = 0; m_rect.top = 0; m_rect.right = 1; m_rect.bottom = 1;
	m_dt_foramt = DT_LEFT | DT_TOP | DT_NOCLIP;
	setConfig(12, 25, 500, false, L"DefaultFont");
}

JGEText::~JGEText()
{
	jgeReleaseCom(m_lpFont);
	jgewcsfree(m_lpStr);
	jgeReleaseCom(m_lpSprite);
}

void JGEText::setText(wchar_t* lpStr)
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
	m_textColor = color;
}

uint JGEText::getTextColor() const
{
	return m_textColor;
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
	m_desc.Width = width; m_desc.Height = height; m_desc.Weight = weight; m_desc.MipLevels = D3DX_DEFAULT; 
	m_desc.Italic = italic; m_desc.CharSet = DEFAULT_CHARSET; m_desc.OutputPrecision = 0; m_desc.Quality = 0;
	jgewcsclone(lpFaceName, m_desc.FaceName);
	resetFont();
}

const D3DXFONT_DESCW* JGEText::getConfig() const
{
	return &m_desc;
}

bool JGEText::setTexture(JGETexture* texture)
{
	return false;
}

JGERect* JGEText::getBounds(JGERect* lpRectResult)
{
	if(lpRectResult == null || jgewcslen(m_lpStr) == 0)
	{
		return null;
	}
	else
	{
		static RECT rect;
		m_lpFont->DrawText(null, m_lpStr, -1, &rect, m_dt_foramt | DT_CALCRECT, m_textColor);
		lpRectResult->m_left = (float)rect.left;
		lpRectResult->m_top = (float)rect.top;
		lpRectResult->m_right = (float)rect.right;
		lpRectResult->m_bottom = (float)rect.bottom;

		return lpRectResult;
	}
}

JGEPoint* JGEText::getBounds(JGEPoint* lpBoundsResult)
{
	return null;
}

void JGEText::resetFont()
{
	jgeReleaseCom(m_lpFont);
	jgeDXVerifyIf(D3DXCreateFontIndirect(getDirect3DDevice(), &m_desc, &m_lpFont))jgeDXVerifyEndIf
}

void JGEText::drawText()
{
	if(jgewcslen(m_lpStr) == 0)
	{
		return;
	}

	if(m_lpSprite == null)
	{
		jgeDXVerifyIf(D3DXCreateSprite(getDirect3DDevice(), &m_lpSprite))jgeDXVerifyEndIf
	}

	static RECT rect;
	rect.left = m_rect.left;
	rect.top = m_rect.top;
	rect.right = m_rect.right;
	rect.bottom = m_rect.bottom;

	float x = 0;
	float y = 0;
	JGEDisplayObjectContainer* lpTarget = getParent();
	while(lpTarget != null)
	{
		x += lpTarget->getX() + lpTarget->getRefX();
		y += lpTarget->getY() + lpTarget->getRefY();
		lpTarget = lpTarget->getParent();
	}
	rect.left += (LONG)x;
	rect.top += (LONG)y;
	rect.right += (LONG)x;
	rect.bottom += (LONG)y;

	static D3DXMATRIX matrix;
	D3DXMatrixTranslation(&matrix, 0.0f, 0.0f, 0.0f);
	m_lpSprite->SetTransform(&matrix);
	m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_lpFont->DrawText(m_lpSprite, m_lpStr, -1, &rect, m_dt_foramt, m_textColor);
	m_lpSprite->End();
}