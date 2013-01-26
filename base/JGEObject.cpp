#include "JGEObject.h"
#include "jgeUtil.h"

JGEObject::JGEObject()
{
	m_lpExtraData = null;
	m_lpName = null;
}

JGEObject::~JGEObject()
{
	jgecsfree(m_lpName);
	m_lpExtraData = null;
}

void JGEObject::setName(const char* lpName)
{
	if(jgecsequ(lpName, m_lpName))
	{
		return;
	}

	jgecsfree(m_lpName);
	m_lpName = jgecsclone(lpName);
}