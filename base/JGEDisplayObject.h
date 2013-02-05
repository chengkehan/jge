#ifndef __JGE_DISPLAYOBJECT_H__
#define __JGE_DISPLAYOBJECT_H__

#include <d3d9.h>
#include "JGEEventDispatcher.h"
#include "JGETexture.h"
#include "JGERect.h"
#include "JGE2DQtree.h"
#include "JGEPoint.h"
#include "jgeUtil.h"

class JGEDisplayObjectContainer;
class JGERender;
class JGE2D;

class JGEDisplayObject : public JGEEventDispatcher, public JGEQtreeNodeData
{
friend class JGERender;
friend class JGEDisplayObjectContainer;
friend class JGE2D;

public:
	struct Vertex
	{
		float x, y, z;
		float rhw;
		D3DCOLOR diffuse;
		float u, v;
		Vertex(){}
		Vertex(float px, float py, D3DCOLOR pdiffuse, float pu, float pv)
		{
			x = px; y = py; z = 0.0f;
			rhw = 0.0f;
			diffuse = pdiffuse;
			u = pu; v = pv;
		}
		static const DWORD FVF;
	};

	JGEDisplayObject(IDirect3DDevice9* lpd3dd);
	~JGEDisplayObject();

	inline void setX(float value) { m_x = value; }
	inline float getX() const { return m_x; }

	inline void setY(float value) { m_y = value; }
	inline float getY() const { return m_y; }

	inline void setRefX(float value) { m_refX = value; }
	inline float getRefX() const { return m_refX; }

	inline void setRefY(float value){ m_refY = value; }
	inline float getRefY() const { return m_refY; }

	inline void setWidth(float value) { m_scaleX = value / m_widthOriginal; }
	inline float getWidth() const { return m_widthOriginal * m_scaleX; }
	inline float getWidthOriginal() const { return m_widthOriginal; }

	inline void setHeight(float value) { m_scaleY = value / m_heightOriginal; }
	inline float getHeight() const { return m_heightOriginal * m_scaleY; }
	inline float getHeightOriginal() const { return m_heightOriginal; }

	inline void setScaleX(float value) { m_scaleX = value; }
	inline float getScaleX() const { return m_scaleX; }

	inline void setScaleY(float value) { m_scaleY = value; }
	inline float getScaleY() const { return m_scaleY; }

	inline void setRotation(float value) { m_rotation = value; }
	inline float getRotation() const { return m_rotation; }

	virtual bool setTexture(JGETexture* texture);
	inline JGETexture* getTexture() const { return m_lpTexture; }

	inline void setAlpha(float value) { m_alpha = min(max(value, 0.0f), 1.0f); }
	inline float getAlpha() const { return m_alpha; }

	inline void setAlphaEnabled(bool value) { m_alphaEnabled = value; }
	inline bool getAlphaEnabled() const { return m_alphaEnabled; }

	inline JGEDisplayObjectContainer* getParent() const { return m_lpParent; }

	virtual JGERect* getBounds(JGERect* lpRectResult);
	virtual JGEPoint* getBounds(JGEPoint* lpBoundsResult);
	static bool inBounds(JGEPoint* lpBounds, JGEPoint* lpPoint);
	static bool inBounds(JGEPoint* lpBounds, float pointX, float pointY);

protected:
	bool m_isContainer;

private:
	JGEDisplayObject();
	JGEDisplayObject(const JGEDisplayObject& value);

	float m_refX;
	float m_refY;
	float m_x;
	float m_y;
	float m_widthOriginal;
	float m_heightOriginal;
	float m_scaleX;
	float m_scaleY;
	float m_rotation;
	JGETexture* m_lpTexture;
	Vertex* m_lpVBData;
	IDirect3DDevice9* m_lpd3dd;
	float m_alpha;
	bool m_alphaEnabled;
	JGEDisplayObjectContainer* m_lpParent;
	uint m_depth;
	uint m_index;

	inline void setParent(JGEDisplayObjectContainer* lpParent) { m_lpParent = lpParent; m_lpParentBubble = (JGEEventDispatcher*)lpParent; }
	void updateVertexBufferData();
};

#endif