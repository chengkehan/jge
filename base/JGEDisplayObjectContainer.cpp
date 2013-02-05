#include <algorithm>
#include "JGEDisplayObjectContainer.h"

using namespace std;

JGEDisplayObjectContainer::JGEDisplayObjectContainer(IDirect3DDevice9* lpd3dd):JGEDisplayObject(lpd3dd)
{
	m_isContainer = true;
}

JGEDisplayObjectContainer::~JGEDisplayObjectContainer()
{

}

JGEDisplayObject* JGEDisplayObjectContainer::addChild(JGEDisplayObject* lpChild)
{
	if(lpChild == null)
	{
		return null;
	}
	if(lpChild->getParent() != null)
	{
		lpChild->getParent()->removeChild(lpChild);
	}
	m_childrenList.push_back(lpChild);
	lpChild->setParent(this);
	lpChild->m_depth = m_depth + 1;
	lpChild->m_index = getNumChildren() - 1;

	return lpChild;
}

JGEDisplayObject* JGEDisplayObjectContainer::addChildAt(JGEDisplayObject* lpChild, uint index)
{
	if(lpChild == null)
	{
		return null;
	}
	if(index > getNumChildren())
	{
		return null;
	}

	if(index == getNumChildren())
	{
		addChild(lpChild);
		return lpChild;
	}
	else
	{
		if(lpChild->getParent() != null)
		{
			lpChild->getParent()->removeChild(lpChild);
		}

		uint indexCount = 0;
		JGEDisplayObject* childAdded = null;
		for (ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(indexCount == index)
			{
				childAdded = lpChild;
				m_childrenList.insert(iter, lpChild);
				break;
			}
		}
		jgeAssert(childAdded != null);
		indexCount = 0;
		for (ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(indexCount != index && childAdded == *iter)
			{
				m_childrenList.erase(iter);
				break;
			}
		}
		childAdded->setParent(this);
		childAdded->m_depth = m_depth + 1;
		childAdded->m_index = index;
		return childAdded;
	}
}

JGEDisplayObject* JGEDisplayObjectContainer::removeChild(JGEDisplayObject* lpChild)
{
	if(!containsChild(lpChild))
	{
		return null;
	}

	ChildrenList::iterator iter = find(m_childrenList.begin(), m_childrenList.end(), lpChild);
	jgeAssert(iter != m_childrenList.end());
	m_childrenList.erase(iter);
	lpChild->setParent(null);
	return lpChild;
}

JGEDisplayObject* JGEDisplayObjectContainer::removeChildAt(uint index)
{
	if(index >= getNumChildren())
	{
		return null;
	}
	else
	{
		uint indexCount = 0;
		for (ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(index == indexCount)
			{
				JGEDisplayObject* lpChild = *iter;
				m_childrenList.erase(iter);
				lpChild->setParent(null);
				return lpChild;
			}
		}

		jgeAssert(false);
		return null;
	}
}

bool JGEDisplayObjectContainer::getChildIndex(JGEDisplayObject* lpChild, uint* lpIndex) const
{
	if(!containsChild(lpChild))
	{
		return false;
	}

	uint index = 0;
	for (ChildrenList::const_iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++index)
	{
		if(lpChild == *iter)
		{
			*lpIndex = index;
			return true;
		}
	}

	jgeAssert(false);
	return false;
}

JGEDisplayObject* JGEDisplayObjectContainer::getChildAt(uint index) const
{
	if(index >= getNumChildren())
	{
		return null;
	}
	else
	{
		uint indexCount = 0;
		for (ChildrenList::const_iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(indexCount == index)
			{
				return *iter;
			}
		}

		jgeAssert(false);
		return null;
	}
}

JGEDisplayObject* JGEDisplayObjectContainer::getChildByName(const char* lpName) const
{
	for (ChildrenList::const_iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		if(jgecsequ(lpName, (*iter)->getName()))
		{
			return *iter;
		}
	}

	return null;
}

JGEDisplayObject* JGEDisplayObjectContainer::setChildIndex(JGEDisplayObject* lpChild, uint index)
{
	if(index >= getNumChildren())
	{
		return null;
	}
	if(!containsChild(lpChild))
	{
		return null;
	}

	return addChildAt(lpChild, index);
}

bool JGEDisplayObjectContainer::setTexture(JGETexture* texture)
{
	return false;
}

JGERect* JGEDisplayObjectContainer::getBounds(JGERect* lpRectResult)
{
	if(lpRectResult == null)
	{
		return null;
	}

	uint numChildren = m_childrenList.size();
	if(numChildren == 0)
	{
		return null;
	}
	else
	{
		ChildrenList::iterator iter = m_childrenList.begin();
		(*iter)->getBounds(lpRectResult);
		JGERect rect;
		for (++iter; iter != m_childrenList.end(); ++iter)
		{
			(*iter)->getBounds(&rect);
			lpRectResult->combine(&rect, lpRectResult);
		}
	}

	return lpRectResult;
}

JGEPoint* JGEDisplayObjectContainer::getBounds(JGEPoint* lpVectorBoundsResult)
{
	return null;
}