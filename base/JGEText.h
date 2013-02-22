#ifndef __JGE_TEXT_H__
#define __JGE_TEXT_H__

#include "JGEDisplayObject.h"

class JGE2D;

class JGEText : public JGEDisplayObject
{
friend class JGE2D;

public:
	JGEText(IDirect3DDevice9* lpd3dd);
	~JGEText();

	void setText(wchar_t* lpStr);
	const wchar_t* getText() const;

	void setTextColor(uint color);
	uint getTextColor() const;

	void setTextBounds(RECT* lpRect);
	const RECT* getTextBounds() const;

	void setTextFormat(uint dt_format);
	uint getTextFormat() const;

	void setConfig(int width = -1, int height = -1, int weight = -1, bool italic = false, const wchar_t* lpFaceName = null);
	const D3DXFONT_DESCW* getConfig() const;

	bool setTexture(JGETexture* texture);

	JGERect* getBounds(JGERect* lpRectResult);
	JGEPoint* getBounds(JGEPoint* lpBoundsResult);

private:
	JGEText();
	JGEText(const JGEText& value);

	ID3DXFont* m_lpFont;
	wchar_t* m_lpStr;
	uint m_textColor;
	RECT m_rect;
	uint m_dt_foramt;
	D3DXFONT_DESCW m_desc;
	ID3DXSprite* m_lpSprite;

	void resetFont();
	void drawText();
};

#endif