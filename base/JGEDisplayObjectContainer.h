#ifndef __JGE_DISPLAYOBJECTCONTAINER_H__
#define __JGE_DISPLAYOBJECTCONTAINER_H__

#include <d3d9.h>
#include <list>
#include "JGEDisplayObject.h"

class JGEDisplayObjectContainer : public JGEDisplayObject
{
public:
	JGEDisplayObjectContainer(IDirect3DDevice9* lpd3dd);
	~JGEDisplayObjectContainer();

	int getNumChildren() const;
	JGEDisplayObject* addChild(JGEDisplayObject* lpChild);
	JGEDisplayObject* addChildAt(JGEDisplayObject* lpChild, int index);
	JGEDisplayObject* removeChild(JGEDisplayObject* lpChild);
	JGEDisplayObject* removeChildAt(int index);
	bool containsChild(JGEDisplayObject* lpChild) const;
	int getChildIndex(JGEDisplayObject* lpChild) const;
	JGEDisplayObject* getChildAt(int index) const;
	JGEDisplayObject* getChildByName(LPCWCHAR name) const;
	JGEDisplayObject* setChildIndex(JGEDisplayObject* lpChild, int index);

private:
	JGEDisplayObjectContainer();
	JGEDisplayObjectContainer(const JGEDisplayObjectContainer& value);

	std::list<JGEDisplayObject*> m_lpChildrenList;
};

#endif