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

	uint getNumChildren() const;
	JGEDisplayObject* addChild(JGEDisplayObject* lpChild);
	JGEDisplayObject* addChildAt(JGEDisplayObject* lpChild, uint index);
	JGEDisplayObject* removeChild(JGEDisplayObject* lpChild);
	JGEDisplayObject* removeChildAt(uint index);
	bool containsChild(JGEDisplayObject* lpChild) const;
	bool getChildIndex(JGEDisplayObject* lpChild, uint* lpIndex) const;
	JGEDisplayObject* getChildAt(uint index) const;
	JGEDisplayObject* getChildByName(const char* lpName) const;
	JGEDisplayObject* setChildIndex(JGEDisplayObject* lpChild, uint index);
	bool setTexture(JGETexture* texture);
	JGERect* getBounds(JGERect* lpRectResult);

private:
	JGEDisplayObjectContainer();
	JGEDisplayObjectContainer(const JGEDisplayObjectContainer& value);

	typedef std::list<JGEDisplayObject*> ChildrenList;

	ChildrenList* m_lpChildrenList;

	void initChildrentList();
};

#endif