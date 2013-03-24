#ifndef __JGE_SCALE9GIRD_DISPLAYOBJECT_H__
#define __JGE_SCALE9GIRD_DISPLAYOBJECT_H__

#include "JGEDisplayObject.h"

class JGEScale9GridDisplayObject : public JGEDisplayObject
{
public:
	JGEScale9GridDisplayObject(IDirect3DDevice9* lpd3dd);
	~JGEScale9GridDisplayObject();

	bool setScale9Grid(const JGERect* lpRect);
	inline const JGERect* getScale9Grid() const { return &m_scale9Grid; }

	virtual JGERect* getBoundsGlobal(JGERect* lpRectResult);

protected:
	virtual void render();
	virtual JGEPoint* getBoundsVectorGlobal(JGEPoint* lpBoundsResult);
	virtual void updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent);
	virtual bool shownInDisplayList();

private:
	JGEScale9GridDisplayObject();
	JGEScale9GridDisplayObject(const JGEScale9GridDisplayObject& value);

	JGERect m_scale9Grid;
	const JGEMatrix2D* m_lpMatrixGlobalParent;
};

#endif