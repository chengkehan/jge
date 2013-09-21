#pragma once

#include "JgeDefinition.h"

#define JGE_SINGLETON_DECLARE(clazz) \
	public: \
	static clazz##* getInstance(); \
	private: \
	clazz##(const clazz##& object); \
	static clazz* m_instance;

#define JGE_SINGLETON_IMPLEMENTS(clazz) \
	clazz* clazz##::m_instance = null; \
	clazz##* clazz##::getInstance() \
	{ \
	if(m_instance == null) \
		{ m_instance = new clazz(); } \
		return m_instance; \
	}