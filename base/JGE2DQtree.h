#ifndef __JGE_2D_QTREE_H__
#define __JGE_2D_QTREE_H__

#include "JGEQtree.h"
#include "jgeUtil.h"

class JGE2DQtree
{
JGE_SINGLETON_DECLARE(JGE2DQtree)
	
public:
	void init(uint depth, uint width, uint height);
	JGEQtree* getQtree();

private:
	JGE2DQtree();
	~JGE2DQtree();

	JGEQtree* m_lpQtree;
};

#endif