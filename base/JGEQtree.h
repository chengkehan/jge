#ifndef __JGE_QTREE_H__
#define __JGE_QTREE_H__

#include <list>
#include <map>
#include "jgeUtil.h"
#include "JGERect.h"

template<class T>
struct JGEQtreeNode
{
	JGERect* lpRect;
	std::map<T*, T*>* lpDataMap;
	JGEQtreeNode<T>* lpSubNodes;
};

template<class T>
class JGEQtree
{
public:
	JGEQtree(uint depth, uint width, uint height);
	~JGEQtree();

	JGEQtreeNode<T>* find(float x, float y);
	bool insert(float x, float y, T* lpData);
	bool insert(float left, float top, float right, float bottom, T* lpData);
	bool earse(T* lpData);
	bool update(float left, float top, float right, float bottom, T* lpData);

private:
	JGEQtree();
	JGEQtree(const JGEQtree& value);

	typedef std::list<JGEQtreeNode<T>*> NodeList;
	typedef std::map<T*, NodeList*> DataMap;

	JGEQtreeNode<T>* m_lpRoot;
	DataMap* m_lpDataMap;
	uint m_width;
	uint m_height;
	uint m_depth;

	void findRecursive(JGEQtreeNode<T>* lpNode, float x, float y, JGEQtreeNode<T>** lplpNodeResult);
	void initNodeRecursive(JGEQtreeNode<T>** lplpNodeParent, float leftParent, float topParent, float rightParent, float bottomParent, uint depth);
	void destroyNodeRecursive(JGEQtreeNode<T>* lpNode);
	void insertRecursive(JGEQtreeNode<T>& lpNode, float x, float y, T* lpData);
	void initDataMap();
	void destroyDataMap();
};

#endif