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

const char* JGEObject::getName() const
{
	return m_lpName;
}

void JGEObject::setExtraData(void* lpData)
{
	m_lpExtraData = lpData;
}

void* JGEObject::getExtraData() const
{
	return m_lpExtraData;
}