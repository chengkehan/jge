#ifndef __JGE_QTREE_H__
#define __JGE_QTREE_H__

#include <hash_map>
#include "JGERect.h"
#include "jgeUtil.h"

class JGEQtreeNodeData;
class JGEQtree;
class JGE2D;

struct JGEQtreeNode
{
	typedef std::hash_map<JGEQtreeNodeData*, JGEQtreeNodeData*> DataMap;

	JGERect rect;
	DataMap* lpDataMap;
	JGEQtreeNode* lpSubNodes[4];
};

class JGEQtreeNodeData
{
friend class JGEQtree;
friend class JGE2D;

public:
	JGEQtreeNodeData();
	~JGEQtreeNodeData();

private:
	JGEQtreeNode* m_lpNode;
	JGEQtreeNodeData* m_lpNodeDataNext;
};

class JGEQtree
{
public:
	JGEQtree(uint depth, uint width, uint height);
	~JGEQtree();

	bool setObject(JGEQtreeNodeData* lpData, const JGERect* lpRect);
	bool clearObject(JGEQtreeNodeData* lpData);
	JGEQtreeNodeData* search(float x, float y);

private:
	JGEQtree();
	JGEQtree(const JGEQtree& value);

	JGEQtreeNode* m_lpRoot;
	uint m_width;
	uint m_height;
	uint m_depth;

	void initRecursive(JGEQtreeNode** lplpNode, uint depth, float left, float top, float right, float bottom);
	void destroyRecursive(JGEQtreeNode* lpNode);
	void setObjectRecursive(JGEQtreeNode* lpNode, JGEQtreeNodeData* lpNodeData, const JGERect* lpRect);
	void searchRecursive(float x, float y, JGEQtreeNode* lpNode, JGEQtreeNodeData** lplpNodeData);
};

#endif