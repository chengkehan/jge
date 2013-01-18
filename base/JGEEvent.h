#ifndef __JGE_EVENT_H__
#define __JGE_EVENT_H__

#include "JGEObject.h"

class JGEEvent : public JGEObject
{
public:
	JGEEvent(int id);
	JGEEvent(const JGEEvent& value);
	~JGEEvent();

	int m_id;
};

#endif