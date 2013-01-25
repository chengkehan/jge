#include "JGE2DQtree.h"

JGE_SINGLETON_IMPLEMENTS(JGE2DQtree)

JGE2DQtree::JGE2DQtree()
{
	m_lpQtree = null;
}

JGE2DQtree::~JGE2DQtree()
{
	jgeDelete(m_lpQtree);
}

void JGE2DQtree::init(uint depth, uint width, uint height)
{
	jgeDelete(m_lpQtree);
	jgeNewArgs3(m_lpQtree, JGEQtree, depth, width, height);
}

JGEQtree* JGE2DQtree::getQtree()
{
	return m_lpQtree;
}