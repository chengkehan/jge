#ifndef __JGE_DISPLAYOBJECTCONTAINER_H__
#define __JGE_DISPLAYOBJECTCONTAINER_H__

#include <d3d9.h>
#include <list>
#include "JGEDisplayObject.h"
#include "JGETexture.h"
#include "JGERect.h"
#include "jgeUtil.h"

class JGE2D;

class JGEDisplayObjectContainer : public JGEDisplayObject
{
friend class JGE2D;

public:
	JGEDisplayObjectContainer(IDirect3DDevice9* lpd3dd);
	~JGEDisplayObjectContainer();

	inline uint getNumChildren() const { return m_childrenList.size(); }
	JGEDisplayObject* addChild(JGEDisplayObject* lpChild);
	JGEDisplayObject* addChildAt(JGEDisplayObject* lpChild, uint index);
	JGEDisplayObject* removeChild(JGEDisplayObject* lpChild);
	JGEDisplayObject* removeChildAt(uint index);
	inline bool containsChild(JGEDisplayObject* lpChild) const { return lpChild != null && lpChild->getParent() == this; }
	bool getChildIndex(JGEDisplayObject* lpChild, uint* lpIndex) const;
	JGEDisplayObject* getChildAt(uint index) const;
	JGEDisplayObject* getChildByName(const char* lpName) const;
	JGEDisplayObject* setChildIndex(JGEDisplayObject* lpChild, uint index);
	bool setTexture(JGETexture* texture);
	JGERect* getBounds(JGERect* lpRectResult);
	JGEPoint* getBounds(JGEPoint* lpVectorBoundsResult);

private:
	JGEDisplayObjectContainer();
	JGEDisplayObjectContainer(const JGEDisplayObjectContainer& value);

	typedef std::list<JGEDisplayObject*> ChildrenList;

	ChildrenList m_childrenList;
};

#endif