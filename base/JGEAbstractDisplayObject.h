#ifndef __JGE_ABSTRACT_DISPLAYOBJECT_H__
#define __JGE_ABSTRACT_DISPLAYOBJECT_H__

#include <d3d9.h>
#include <d3dx9.h>
#include "JGEEventDispatcher.h"
#include "JGEVector2D.h"
#include "JGEMatrix2D.h"
#include "JGERect.h"
#include "jgeUtil.h"
#include "JGE2DQtree.h"

class JGEDisplayObjectContainer;
class JGE2D;

class JGEAbstractDisplayObject : public JGEEventDispatcher, public JGEQtreeNodeData
{
friend class JGEDisplayObjectContainer;
friend class JGE2D;

public:
	JGEAbstractDisplayObject(IDirect3DDevice9* lpd3dd);
	~JGEAbstractDisplayObject();

	inline void setX(float value) { m_x = value; }
	inline float getX() const { return m_x; }

	inline void setY(float value) { m_y = value; }
	inline float getY() const { return m_y; }

	inline void setWidth(float value) { m_scaleX = value / m_widthOriginal; }
	inline float getWidth() const { return m_widthOriginal * m_scaleX; }

	inline void setHeight(float value) { m_scaleY = value / m_heightOriginal; }
	inline float getHeight() const { return m_heightOriginal * m_scaleY; }

	inline void setScaleX(float value) { m_scaleX = value; }
	inline float getScaleX() const { return m_scaleX; }

	inline void setScaleY(float value) { m_scaleY = value; }
	inline float getScaleY() const { return m_scaleY; }

	inline void setRotation(float value) { m_rotation = value; }
	inline float getRotation() const { return m_rotation; }

	inline virtual void setAlpha(float value) { m_alpha = min(max(value, 0.0f), 1.0f); }
	inline float getAlpha() const { return m_alpha; }

	inline void setVisible(bool value) { m_visible = value; }
	inline bool getVisible() const { return m_visible; }

	inline JGEDisplayObjectContainer* getParent() const { return m_lpParent; }
	inline IDirect3DDevice9* getDirect3DDevice() { return m_lpd3dd; }

	virtual JGERect* getBoundsGlobal(JGERect* lpRectResult) = 0;
	virtual bool inBoundsGlobal(float x, float y) = 0;

protected:
	float m_widthOriginal, m_heightOriginal;
	JGEMatrix2D m_matrixGlobal;

	virtual void render() = 0;
	virtual bool shownInDisplayList() = 0;
	virtual void qtreeSet() = 0;
	virtual void qtreeClear() = 0;
	virtual void qtreeSetClear() = 0;
	virtual void updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent) = 0;

private:
	JGEAbstractDisplayObject();
	JGEAbstractDisplayObject(const JGEAbstractDisplayObject& value);

	float m_x, m_y;
	float m_scaleX, m_scaleY;
	float m_rotation;
	float m_alpha;
	JGEDisplayObjectContainer* m_lpParent;
	IDirect3DDevice9* m_lpd3dd;
	bool m_visible;
	uint m_depth, m_index;

	inline void setParent(JGEDisplayObjectContainer* lpParent) { m_lpParent = lpParent; m_lpParentBubble = (JGEEventDispatcher*)lpParent; }
};

#endif