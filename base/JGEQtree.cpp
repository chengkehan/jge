#include "JGEQtree.h"

JGEQtreeNodeData::JGEQtreeNodeData()
{
	m_lpNodeDataNext = null;
	m_lpNode = null;
}

JGEQtreeNodeData::~JGEQtreeNodeData()
{
	m_lpNodeDataNext = null;
	m_lpNode = null;
}

JGEQtree::JGEQtree(uint depth, uint width, uint height)
{
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_lpRoot = null;
	initRecursive(&m_lpRoot, 1, 0.0f, 0.0f, (float)width, (float)height);
}

JGEQtree::~JGEQtree()
{
	destroyRecursive(m_lpRoot);
	m_lpRoot = null;
}

bool JGEQtree::setObject(JGEQtreeNodeData* lpData, const JGERect* lpRect)
{
	if(m_lpRoot == null || lpData == null || lpRect == null)
	{
		return false;
	}
	else
	{
		setObjectRecursive(m_lpRoot, lpData, lpRect);
		return true;
	}
}

bool JGEQtree::clearObject(JGEQtreeNodeData* lpData)
{
	if(lpData == null || lpData->m_lpNode == null || lpData->m_lpNode->lpDataSet == null)
	{
		return false;
	}
	else
	{
		size_t r = lpData->m_lpNode->lpDataSet->erase(lpData);
		jgeAssert(r != 0);
		lpData->m_lpNode = null;
		return true;
	}
}

JGEQtreeNodeData* JGEQtree::search(float x, float y)
{
	JGEQtreeNodeData* lpNodeData = null;
	searchRecursive(x, y, m_lpRoot, &lpNodeData);
	return lpNodeData;
}

void JGEQtree::initRecursive(JGEQtreeNode** lplpNode, uint depth, float left, float top, float right, float bottom)
{
	*lplpNode = null;

	if(depth > m_depth)
	{
		return;
	}

	jgeNew(*lplpNode, JGEQtreeNode);

	(*lplpNode)->rect.m_left = left;
	(*lplpNode)->rect.m_top = top;
	(*lplpNode)->rect.m_right = right;
	(*lplpNode)->rect.m_bottom = bottom;

	(*lplpNode)->lpDataSet = null;

	float widthHalf = (right - left) * 0.5f;
	float heightHalf = (bottom - top) * 0.5f;		
	initRecursive(&(*lplpNode)->lpSubNodes[0], depth + 1, left, top, left + widthHalf, top + heightHalf);
	initRecursive(&(*lplpNode)->lpSubNodes[1], depth + 1, left + widthHalf, top, left + widthHalf * 2.0f, top + heightHalf);
	initRecursive(&(*lplpNode)->lpSubNodes[2], depth + 1, left, top + heightHalf, left + widthHalf, top + heightHalf * 2.0f);
	initRecursive(&(*lplpNode)->lpSubNodes[3], depth + 1, left + widthHalf, top + heightHalf, left + widthHalf * 2.0f, top + heightHalf * 2.0f);
}

void JGEQtree::destroyRecursive(JGEQtreeNode* lpNode)
{
	if(lpNode == null)
	{
		return;
	}

	if(lpNode->lpDataSet != null)
	{
		for (JGEQtreeNode::DataSet::iterator iter = lpNode->lpDataSet->begin(); iter != lpNode->lpDataSet->end(); ++iter)
		{
			(*iter)->m_lpNode = null;
		}
	}
	jgeDelete(lpNode->lpDataSet);
	if(lpNode->lpSubNodes != null)
	{
		destroyRecursive(lpNode->lpSubNodes[0]);
		destroyRecursive(lpNode->lpSubNodes[1]);
		destroyRecursive(lpNode->lpSubNodes[2]);
		destroyRecursive(lpNode->lpSubNodes[3]);
	}
	jgeDelete(lpNode);
}

void JGEQtree::setObjectRecursive(JGEQtreeNode* lpNode, JGEQtreeNodeData* lpNodeData, const JGERect* lpRect)
{
	if(lpNode != null && lpRect != null && !lpNode->rect.contains(lpRect))
	{
		return;
	}

	if(lpNode->lpSubNodes[0] == null)
	{
		if(lpNode->lpDataSet == null)
		{
			jgeNew(lpNode->lpDataSet, JGEQtreeNode::DataSet);
		}
		if(lpNodeData->m_lpNode != null && lpNodeData->m_lpNode->lpDataSet != null)
		{
			lpNodeData->m_lpNode->lpDataSet->erase(lpNodeData);
		}
		lpNode->lpDataSet->insert(lpNodeData);
		lpNodeData->m_lpNode = lpNode;
	}
	else
	{
		if(lpNode->lpSubNodes[0]->rect.contains(lpRect))
		{
			setObjectRecursive(lpNode->lpSubNodes[0], lpNodeData, lpRect);
			return;
		}
		if(lpNode->lpSubNodes[1]->rect.contains(lpRect))
		{
			setObjectRecursive(lpNode->lpSubNodes[1], lpNodeData, lpRect);
			return;
		}
		if(lpNode->lpSubNodes[2]->rect.contains(lpRect))
		{
			setObjectRecursive(lpNode->lpSubNodes[2], lpNodeData, lpRect);
			return;
		}
		if(lpNode->lpSubNodes[3]->rect.contains(lpRect))
		{
			setObjectRecursive(lpNode->lpSubNodes[3], lpNodeData, lpRect);
			return;
		}

		if(lpNode->lpDataSet == null)
		{
			jgeNew(lpNode->lpDataSet, JGEQtreeNode::DataSet);
		}
		if(lpNodeData->m_lpNode != null && lpNodeData->m_lpNode->lpDataSet != null)
		{
			lpNodeData->m_lpNode->lpDataSet->erase(lpNodeData);
		}
		lpNode->lpDataSet->insert(lpNodeData);
		lpNodeData->m_lpNode = lpNode;
	}
}

void JGEQtree::searchRecursive(float x, float y, JGEQtreeNode* lpNode, JGEQtreeNodeData** lplpNodeData)
{
	if(lpNode == null || !lpNode->rect.contains(x, y))
	{
		return;
	}

	if(lpNode->lpDataSet != null)
	{
		for(JGEQtreeNode::DataSet::iterator iter = lpNode->lpDataSet->begin(); iter != lpNode->lpDataSet->end(); ++iter)
		{
			JGEQtreeNodeData* lpDataNext = *lplpNodeData;
			*lplpNodeData = *iter;
			(*lplpNodeData)->m_lpNodeDataNext = lpDataNext;
		}
	}

	if(lpNode->lpSubNodes[0] != null)
	{
		searchRecursive(x, y, lpNode->lpSubNodes[0], &(*lplpNodeData)->m_lpNodeDataNext);
		searchRecursive(x, y, lpNode->lpSubNodes[1], &(*lplpNodeData)->m_lpNodeDataNext);
		searchRecursive(x, y, lpNode->lpSubNodes[2], &(*lplpNodeData)->m_lpNodeDataNext);
		searchRecursive(x, y, lpNode->lpSubNodes[3], &(*lplpNodeData)->m_lpNodeDataNext);
	}
}