#ifndef __JGE_TEXT_H__
#define __JGE_TEXT_H__

#include "JGEAbstractDisplayObject.h"

class JGEText : public JGEAbstractDisplayObject
{
public:
	JGEText(IDirect3DDevice9* lpd3dd);
	~JGEText();

	inline virtual void setAlpha(float value) { JGEAbstractDisplayObject::setAlpha(value); updateColor(); }

	void setText(const wchar_t* lpStr);
	const wchar_t* getText() const;

	void setTextColor(uint color);
	uint getTextColor() const;

	void setTextBounds(const RECT* lpRect);
	const RECT* getTextBounds() const;

	void setTextFormat(uint dt_format);
	uint getTextFormat() const;

	void setConfig(int width = -1, int height = -1, int weight = -1, bool italic = false, const wchar_t* lpFaceName = null);
	const D3DXFONT_DESCW* getConfig() const;

	virtual JGERect* getBoundsGlobal(JGERect* lpRectResult);
	virtual bool inBoundsGlobal(float x, float y);

protected:
	virtual void render();
	virtual bool shownInDisplayList();
	virtual void qtreeSet();
	virtual void qtreeClear();
	virtual void qtreeSetClear();
	virtual void updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent);

private:
	JGEText();
	JGEText(const JGEText& value);

	ID3DXFont* m_lpFont;
	wchar_t* m_lpStr;
	uint m_color;
	uint m_textColor;
	RECT m_rect;
	uint m_dt_foramt;
	D3DXFONT_DESCW m_desc;
	ID3DXSprite* m_lpSprite;

	static JGEPoint m_boundsTemp[4];

	void resetFont();
	inline void updateColor() { m_textColor = (((int)(getAlpha() * 255.0) & 0xFF) << 24) + (m_color & 0xFFFFFF); }
};

#endif