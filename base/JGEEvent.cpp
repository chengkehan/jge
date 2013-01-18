#include "JGEEvent.h"

JGEEvent::JGEEvent(int id)
{
	m_id = id;
}

JGEEvent::JGEEvent(const JGEEvent& value)
{
	m_id = value.m_id;
}

JGEEvent::~JGEEvent()
{
	// Do nothing
}