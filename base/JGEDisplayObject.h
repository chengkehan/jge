#ifndef __JGE_DISPLAYOBJECT_H__
#define __JGE_DISPLAYOBJECT_H__

#include "JGEAbstractDisplayObject.h"
#include "JGETexture.h"

class JGERender;

class JGEDisplayObject : public JGEAbstractDisplayObject
{
friend class JGERender;

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

	inline void setRefX(float value) { m_refX = value; }
	inline float getRefX() const { return m_refX; }

	inline void setRefY(float value){ m_refY = value; }
	inline float getRefY() const { return m_refY; }

	bool setTexture(JGETexture* texture);
	inline JGETexture* getTexture() const { return m_lpTexture; }

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
	JGEDisplayObject();
	JGEDisplayObject(const JGEDisplayObject& value);

	float m_refX;
	float m_refY;
	JGETexture* m_lpTexture;
	Vertex* m_lpVBData;

	JGEPoint* getBoundsVectorGlobal(JGEPoint* lpBoundsResult);
};

#endif