#include <algorithm>
#include "JGEDisplayObjectContainer.h"

using namespace std;

JGEDisplayObjectContainer::JGEDisplayObjectContainer(IDirect3DDevice9* lpd3dd):JGEAbstractDisplayObject(lpd3dd)
{
	m_interactive = false;
}

JGEDisplayObjectContainer::~JGEDisplayObjectContainer()
{

}

JGEAbstractDisplayObject* JGEDisplayObjectContainer::addChild(JGEAbstractDisplayObject* lpChild)
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

JGEAbstractDisplayObject* JGEDisplayObjectContainer::addChildAt(JGEAbstractDisplayObject* lpChild, uint index)
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
		JGEAbstractDisplayObject* childAdded = null;
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

JGEAbstractDisplayObject* JGEDisplayObjectContainer::removeChild(JGEAbstractDisplayObject* lpChild)
{
	if(!containsChild(lpChild))
	{
		return null;
	}

	ChildrenList::iterator iter = find(m_childrenList.begin(), m_childrenList.end(), lpChild);
	jgeAssert(iter != m_childrenList.end());
	m_childrenList.erase(iter);
	lpChild->setParent(null);
	lpChild->qtreeClear();
	return lpChild;
}

JGEAbstractDisplayObject* JGEDisplayObjectContainer::removeChildAt(uint index)
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
				JGEAbstractDisplayObject* lpChild = *iter;
				m_childrenList.erase(iter);
				lpChild->setParent(null);
				lpChild->qtreeClear();
				return lpChild;
			}
		}

		jgeAssert(false);
		return null;
	}
}

int JGEDisplayObjectContainer::getChildIndex(JGEAbstractDisplayObject* lpChild) const
{
	if(!containsChild(lpChild))
	{
		return -1;
	}

	uint index = 0;
	for (ChildrenList::const_iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++index)
	{
		if(lpChild == *iter)
		{
			return index;
		}
	}

	jgeAssert(false);
	return -1;
}

JGEAbstractDisplayObject* JGEDisplayObjectContainer::getChildAt(uint index) const
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

JGEAbstractDisplayObject* JGEDisplayObjectContainer::getChildByName(const char* lpName) const
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

JGEAbstractDisplayObject* JGEDisplayObjectContainer::setChildIndex(JGEAbstractDisplayObject* lpChild, uint index)
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

JGERect* JGEDisplayObjectContainer::getBoundsGlobal(JGERect* lpRectResult)
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
		bool boundsGot = false;
		static JGERect rect;
		for (ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
		{
			if((*iter)->getBoundsGlobal(&rect) != null)
			{
				if(boundsGot)
				{
					lpRectResult->combine(&rect, lpRectResult);
				}
				else
				{
					boundsGot = true;
					lpRectResult->copy(&rect);
				}
			}
		}

		return boundsGot ? lpRectResult : null;
	}
}

bool JGEDisplayObjectContainer::inBoundsGlobal(float x, float y)
{
	if(m_childrenList.size() <= 0)
	{
		return false;
	}

	static JGERect childBounds;
	for(ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		if((*iter)->getBoundsGlobal(&childBounds) != null && childBounds.contains(x, y))
		{
			return true;
		}
	}

	return false;
}

void JGEDisplayObjectContainer::render()
{
	if(!shownInDisplayList())
	{
		return;
	}

	for (ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		(*iter)->render();
	}
}

bool JGEDisplayObjectContainer::shownInDisplayList()
{
	return getVisible() && m_childrenList.size() > 0;
}

void JGEDisplayObjectContainer::qtreeSet()
{
	for(ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		(*iter)->qtreeSetClear();
	}
}

void JGEDisplayObjectContainer::qtreeClear()
{
	for(ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		(*iter)->qtreeClear();
	}
}

void JGEDisplayObjectContainer::qtreeSetClear()
{
	if(shownInDisplayList())
	{
		qtreeSet();
	}
	else
	{
		qtreeClear();
	}
}

void JGEDisplayObjectContainer::updateMatrixGlobal(const JGEMatrix2D* lpMatrixGlobalParent)
{
	if(lpMatrixGlobalParent == null && !shownInDisplayList())
	{
		return;
	}

	jgeMatrix2DRotationScalingTranslationDotProductAlpha(m_rotation, m_scaleX, m_scaleY, m_x, m_y, lpMatrixGlobalParent, getAlpha(), &m_matrixGlobal);
	for(ChildrenList::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		(*iter)->updateMatrixGlobal(&m_matrixGlobal);
	}
}