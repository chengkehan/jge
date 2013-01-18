#ifndef __JGE_EVENTDISPATCHER_H__
#define __JGE_EVENTDISPATCHER_H__

#include <list>
#include <map>
#include "JGEObject.h"
#include "JGEEvent.h"

class JGEEventDispatcher : public JGEObject
{
public:
	typedef void (*EventHandler)(JGEEvent* lpEvent);

	JGEEventDispatcher();
	~JGEEventDispatcher();

	bool addEventListener(int eventID, EventHandler handler);
	bool removeEventListener(int eventID, EventHandler handler);
	bool hasEventListener(int eventID);
	bool dispatchEvent(JGEEvent* lpEvent, bool bubble = false);

protected:
	JGEEventDispatcher* m_lpParent;

private:
	typedef std::list<EventHandler> EventHandlerList;
	typedef std::map<int, EventHandlerList*> EventMap;

	JGEEventDispatcher(const JGEEventDispatcher& value);
	inline void initEventMap();

	EventMap* m_lpEventMap;
};

#endif