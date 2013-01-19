#ifndef __JGE_DISPLAYOBJECT_H__
#define __JGE_DISPLAYOBJECT_H__

#include <d3d9.h>
#include "JGEEventDispatcher.h"
#include "JGETexture.h"

class JGEDisplayObjectContainer;
class JGERender;
class JGE2D;

class JGEDisplayObject : public JGEEventDispatcher
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
		static CONST DWORD FVF;
	};

	JGEDisplayObject(IDirect3DDevice9* lpd3dd);
	~JGEDisplayObject();

	void setX(float value);
	float getX() const;

	void setY(float value);
	float getY() const;

	void setRefX(float value);
	float getRefX() const;

	void setRefY(float value);
	float getRefY() const;

	void setWidth(float value);
	float getWidth() const;
	float getWidthOriginal() const;

	void setHeight(float value);
	float getHeight() const;
	float getHeightOriginal() const;

	void setScaleX(float value);
	float getScaleX() const;

	void setScaleY(float value);
	float getScaleY() const;

	void setRotation(float value);
	float getRotation() const;

	void setTexture(JGETexture* texture);
	JGETexture* getTexture() const;

	void setAlpha(float value);
	float getAlpha() const;

	void setAlphaEnabled(bool value);
	bool getAlphaEnabled() const;

	JGEDisplayObjectContainer* getParent() const;

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

	void setParent(JGEDisplayObjectContainer* lpParent);
	void updateVertexBufferData();
	inline void initVBData();
};

#endif