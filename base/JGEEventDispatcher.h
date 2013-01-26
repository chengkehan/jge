#ifndef __JGE_EVENTDISPATCHER_H__
#define __JGE_EVENTDISPATCHER_H__

#include <list>
#include <map>
#include "JGEObject.h"
#include "JGEEvent.h"
#include "jgeUtil.h"

class JGEEventDispatcher : public JGEObject
{
public:
	typedef void (*EventHandler)(JGEEvent* lpEvent);

	JGEEventDispatcher();
	~JGEEventDispatcher();

	bool addEventListener(int eventID, EventHandler handler);
	bool removeEventListener(int eventID, EventHandler handler);
	bool hasEventListener(int eventID) const;
	bool dispatchEvent(JGEEvent* lpEvent, bool bubble = false);

protected:
	JGEEventDispatcher* m_lpParentBubble;

private:
	typedef std::list<EventHandler> EventHandlerList;
	typedef std::map<int, EventHandlerList*> EventMap;

	JGEEventDispatcher(const JGEEventDispatcher& value);

	EventMap* m_lpEventMap;

	inline void initEventMap() { if(m_lpEventMap == null){ jgeNew(m_lpEventMap, EventMap); } }
};

#endif