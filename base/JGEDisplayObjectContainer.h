#ifndef __JGE_DISPLAYOBJECTCONTAINER_H__
#define __JGE_DISPLAYOBJECTCONTAINER_H__

#include <list>
#include "JGEAbstractDisplayObject.h"

class JGE2D;

class JGEDisplayObjectContainer : public JGEAbstractDisplayObject
{
friend class JGE2D;

public:
	JGEDisplayObjectContainer(IDirect3DDevice9* lpd3dd);
	~JGEDisplayObjectContainer();

	inline uint getNumChildren() const { return m_childrenList.size(); }
	JGEAbstractDisplayObject* addChild(JGEAbstractDisplayObject* lpChild);
	JGEAbstractDisplayObject* addChildAt(JGEAbstractDisplayObject* lpChild, uint index);
	JGEAbstractDisplayObject* removeChild(JGEAbstractDisplayObject* lpChild);
	JGEAbstractDisplayObject* removeChildAt(uint index);
	inline bool containsChild(JGEAbstractDisplayObject* lpChild) const { return lpChild != null && lpChild->getParent() == this; }
	int getChildIndex(JGEAbstractDisplayObject* lpChild) const;
	JGEAbstractDisplayObject* getChildAt(uint index) const;
	JGEAbstractDisplayObject* getChildByName(const char* lpName) const;
	JGEAbstractDisplayObject* setChildIndex(JGEAbstractDisplayObject* lpChild, uint index);
	
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
	JGEDisplayObjectContainer();
	JGEDisplayObjectContainer(const JGEDisplayObjectContainer& value);

	typedef std::list<JGEAbstractDisplayObject*> ChildrenList;

	ChildrenList m_childrenList;
};

#endif