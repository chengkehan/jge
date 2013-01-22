#include "JGEQtree.h"

using namespace std;

template<class T>
JGEQtree<T>::JGEQtree(uint depth, uint width, uint height)
{
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_lpRoot = null;

	initNodeRecursive(&m_lpRoot, 0.0f, 0.0f, (float)m_width, (float)m_height, 1);
}

template<class T>
JGEQtree<T>::~JGEQtree()
{
	destroyDataMap();
	destroyNodeRecursive(m_lpRoot);
	m_lpRoot = null;
}

template<class T>
JGEQtreeNode<T>* JGEQtree<T>::find(float x, float y)
{
	JGEQtreeNode<T>* lpNode = null;
	findRecursive(m_lpRoot, x, y, &lpNode);
	return lpNode;
}

template<class T>
bool JGEQtree<T>::insert(float x, float y, T* lpData)
{
	if(m_lpRoot == null || lpData == null || !m_lpRoot->lpRect->contains(x, y))
	{
		return false;
	}

	insertRecursive(m_lpRoot, x, y, lpData);
	return true;
}

template<class T>
void JGEQtree<T>::findRecursive(JGEQtreeNode<T>* lpNode, float x, float y, JGEQtreeNode<T>** lplpNodeResult)
{
	if(lpNode != null && lpNode->lpRect->contains(x, y))
	{
		if(lpNode->lpSubNodes == null)
		{
			*lplpNodeResult = lpNode;
		}
		else
		{
			for (int i = 0; i < 4; ++i)
			{
				findRecursive(&lpNode->lpSubNodes[i], x, y, lplpNodeResult);
			}
		}
	}
}

template<class T>
void JGEQtree<T>::initNodeRecursive(JGEQtreeNode<T>** lplpNodeParent, float leftParent, float topParent, float rightParent, float bottomParent, uint depth)
{
	if(depth > m_depth)
	{
		if(*lplpNodeParent != null)
		{
			*lplpNodeParent->lpSubNodes = null;
			*lplpNodeParent->lpDataMap = null;
		}
	}
	else
	{
		float widthHalf = (rightParent - leftParent) * 0.5f;
		float heightHalf = (bottomParent - topParent) * 0.5f;

		*lplpNodeParent->lpDataMap = null;

		*lplpNodeParent->lpRect = null;
		jgeNew(*lplpNodeParent->lpRect, JGERect);
		*lplpNodeParent->lpRect->m_left = leftParent;
		*lplpNodeParent->lpRect->m_top = topParent;
		*lplpNodeParent->lpRect->m_right = rightParent;
		*lplpNodeParent->lpRect->m_bottom = bottomParent;

		*lplpNodeParent->lpSubNodes = null;
		jgeNewArray(*lplpNodeParent->lpSubNodes, JGEQtreeNode<T>, 4);
		initNodeRecursive(&&(*lplpNodeParent->lpSubNodes[0]), leftParent, topParent, leftParent + widthHalf, topParent + heightHalf, depth + 1);
		initNodeRecursive(&&(*lplpNodeParent->lpSubNodes[1]), leftParent + widthHalf, topParent, leftParent + widthHalf * 2.0f, topParent + heightHalf, depth + 1);
		initNodeRecursive(&&(*lplpNodeParent->lpSubNodes[2]), leftParent + widthHalf, topParent + heightHalf, leftParent + widthHalf * 2.0f, topParent + heightHalf * 2.0f, depth + 1);
		initNodeRecursive(&&(*lplpNodeParent->lpSubNodes[3]), leftParent, topParent + heightHalf, leftParent + widthHalf, topParent + heightHalf * 2.0f, depth + 1);
	}
}

template<class T>
void JGEQtree<T>::destroyNodeRecursive(JGEQtreeNode<T>* lpNode)
{
	if(lpNode != null)
	{
		jgeDelete(lpNode->lpRect);
		jgeDelete(lpNode->lpDataMap);

		if(lpNode->lpSubNodes != null)
		{
			for (int i = 0; i < 4; ++i)
			{
				destroyNodeRecursive(&lpNode->lpSubNodes[i]);
			}
			jgeDeleteArray(lpNode->lpSubNodes);
		}
	}
}

template<class T>
void JGEQtree<T>::insertRecursive(JGEQtreeNode<T>& lpNode, float x, float y, T* lpData)
{
	if(lpNode != null && lpNode->lpRect->contains(x, y))
	{
		if(lpNode->lpSubNodes == null)
		{
			if(lpNode->lpDataMap == null)
			{
				lpNode->lpDataMap = new map<T*, T*>();
				jgeAssert(lpNode != null);
			}
			lpNode->lpDataMap->insert(map<T*, T*>::value_type(lpData, lpData));
		}
	}
}

template<class T>
inline void JGEQtree<T>::initDataMap()
{
	if(m_lpDataMap == null)
	{
		jgeNew(m_lpDataMap, DataMap);
	}
}

template<class T>
inline void JGEQtree<T>::destroyDataMap()
{
	for(DataMap::iterator iter = m_lpDataMap->begin(); iter != m_lpDataMap->end(); ++iterator)
	{

	}
}