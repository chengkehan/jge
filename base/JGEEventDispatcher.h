#ifndef __JGE_EVENTDISPATCHER_H__
#define __JGE_EVENTDISPATCHER_H__

#include <list>
#include <map>
#include "JGEObject.h"
#include "JGEEvent.h"
#include "jgeUtil.h"
#include "JGECallback.h"

class JGEEventDispatcher : public JGEObject
{
public:
	typedef JGEAbstractCallback<JGEEvent>* EventHandler;

	JGEEventDispatcher();
	~JGEEventDispatcher();

	bool addEventListener(int eventID, JGECallbackStd<JGEEvent>& handler);
	template<class This>
	bool addEventListener(int eventID, JGECallbackThis<This, JGEEvent>& handler)
	{
		initEventMap();

		EventMap::iterator iter = m_lpEventMap->find(eventID);
		EventHandlerList* lpHandlerList = null;
		// new handlerList
		if(iter == m_lpEventMap->end())
		{
			jgeNew(lpHandlerList, EventHandlerList);
			(*m_lpEventMap)[eventID] = lpHandlerList;
		}

		// push a handler to handlerList
		JGECallbackThis<This, JGEEvent>* lpHandler = new JGECallbackThis<This, JGEEvent>();
		jgeAssert(lpHandler != null);
		handler.clone(lpHandler);
		if(lpHandlerList == null)
		{
			iter->second->push_back(lpHandler);
		}
		else
		{
			lpHandlerList->push_back(lpHandler);
		}

		return true;
	}
	bool removeEventListener(int eventID, JGECallbackStd<JGEEvent>& handler);
	template<class This>
	bool removeEventListener(int eventID, JGECallbackThis<This, JGEEvent>& handler)
	{
		return removeEventListenerInternal(eventID, &handler);
	}
	bool hasEventListener(int eventID) const;
	bool dispatchEvent(JGEEvent* lpEvent, bool bubble = false);
	void removeEventListeners();

protected:
	JGEEventDispatcher* m_lpParentBubble;

private:
	typedef std::list<EventHandler> EventHandlerList;
	typedef std::map<int, EventHandlerList*> EventMap;

	JGEEventDispatcher(const JGEEventDispatcher& value);

	EventMap* m_lpEventMap;

	inline void initEventMap() { if(m_lpEventMap == null){ jgeNew(m_lpEventMap, EventMap); } }
	void destroyEventMap();
	bool removeEventListenerInternal(int eventID, JGEAbstractCallback<JGEEvent>* lpHandler);
};

#endif