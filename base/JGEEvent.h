#ifndef __JGE_EVENT_H__
#define __JGE_EVENT_H__

#include "JGEObject.h"

class JGEEventDispatcher;

class JGEEvent : public JGEObject
{
public:
	JGEEvent(int id);
	~JGEEvent();

	int m_id;
	JGEEventDispatcher* m_lpEventDispatcher;

	static int MOUSE_DOWN_LEFT;
	static int MOUSE_DOWN_RIGHT;
	static int MOUSE_DOWN_MIDDLE;
	static int MOUSE_UP_LEFT;
	static int MOUSE_UP_RIGHT;
	static int MOUSE_UP_MIDDLE;
	static int MOUSE_CLICK_LEFT;
	static int MOUSE_CLICK_RIGHT;
	static int MOUSE_CLICK_MIDDLE;
	static int MOUSE_OVER;
	static int MOUSE_OUT;

private:
	JGEEvent();
	JGEEvent(const JGEEvent& value);
};

#endif