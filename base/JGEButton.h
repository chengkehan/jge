#ifndef __JGE_BUTTON_H__
#define __JGE_BUTTON_H__

#include "JGEAbstractDisplayObject.h"
#include "JGEText.h"

class JGEButton : public JGEAbstractDisplayObject
{
public:
	JGEButton(IDirect3DDevice9* lpd3dd);
	~JGEButton();

	virtual JGERect* getBoundsGlobal(JGERect* lpRectResult);
	virtual bool inBoundsGlobal(float x, float y);

	void setLabel(const wchar_t* lpLabel);
	inline const wchar_t* getLabel() const { return m_lpLabel == null ? null : m_lpLabel->getText(); }

	inline void setLabelColor(uint color) { if(m_lpLabel != null) { m_lpLabel->setTextColor(color); } }
	inline uint getLabelColor() const { return m_lpLabel == null ? 0x000000 : m_lpLabel->getTextColor(); }

	inline void setLabelAlpha(float alpha) { if(m_lpLabel != null) { m_lpLabel->setAlpha(alpha); } }
	inline float getLabelAlpha() const { return m_lpLabel == null ? 0.0f : m_lpLabel->getAlpha(); }

	inline void setLabelConfig(int width = -1, int height = -1, int weight = -1, bool italic = false, const wchar_t* lpFaceName = null) { if(m_lpLabel != null) { m_lpLabel->setConfig(width, height, weight, italic, lpFaceName); } }
	inline const D3DXFONT_DESCW* getLabelConfig() const { return m_lpLabel == null ? null : m_lpLabel->getConfig(); }

	inline void setOutSkin(JGEAbstractDisplayObject* lpSkin) { skinQtreeClear(m_lpOutSkin); m_lpOutSkin = lpSkin; }
	inline JGEAbstractDisplayObject* getOutSkin() const { return m_lpOutSkin; }

	inline void setOverSkin(JGEAbstractDisplayObject* lpSkin) { skinQtreeClear(m_lpOverSkin); m_lpOverSkin = lpSkin; }
	inline JGEAbstractDisplayObject* getOverSkin() const { return m_lpOverSkin; }

	inline void setDownSkin(JGEAbstractDisplayObject* lpSkin) { skinQtreeClear(m_lpDownSkin); m_lpDownSkin = lpSkin; }
	inline JGEAbstractDisplayObject* getDownSkin() const { return m_lpDownSkin; }

	inline void setDisabledSkin(JGEAbstractDisplayObject* lpSkin) { skinQtreeClear(m_lpDisabledSkin); m_lpDisabledSkin = lpSkin; }
	inline JGEAbstractDisplayObject* getDisabledSkin() const { return m_lpDisabledSkin; }

protected:
	virtual void render();
	virtual bool shownInDisplayList();
	virtual void qtreeSet();
	virtual void qtreeClear();
	virtual void qtreeSetClear();
	virtual void updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent);

private:
	JGEButton();
	JGEButton(const JGEButton& value);

	JGEAbstractDisplayObject* m_lpOutSkin;
	JGEAbstractDisplayObject* m_lpOverSkin;
	JGEAbstractDisplayObject* m_lpDownSkin;
	JGEAbstractDisplayObject* m_lpDisabledSkin;
	JGEAbstractDisplayObject* m_lpCurrentSkin;

	JGEText* m_lpLabel;

	void updateLabelBounds();
	inline void skinQtreeClear(JGEAbstractDisplayObject* lpSkin) { if(lpSkin != null){ lpSkin->qtreeClear(); } }
};

#endif