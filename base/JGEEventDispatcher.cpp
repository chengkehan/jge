#include <algorithm>
#include "JGEEventDispatcher.h"

using namespace std;

JGEEventDispatcher::JGEEventDispatcher()
{
	m_lpEventMap = null;
	m_lpParentBubble = null;
}

JGEEventDispatcher::~JGEEventDispatcher()
{
	destroyEventMap();
	m_lpParentBubble = null;
}

bool JGEEventDispatcher::addEventListener(int eventID, JGECallbackStd<JGEEvent>& handler)
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
	JGECallbackStd<JGEEvent>* lpHandler = null;
	jgeNew(lpHandler, JGECallbackStd<JGEEvent>);
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

bool JGEEventDispatcher::removeEventListener(int eventID, JGECallbackStd<JGEEvent>& handler)
{
	return removeEventListenerInternal(eventID, &handler);
}

bool JGEEventDispatcher::removeEventListenerInternal(int eventID, EventHandler lpHandler)
{
	if(m_lpEventMap == null || lpHandler == null)
	{
		return false;
	}

	EventMap::iterator iterMap = m_lpEventMap->find(eventID);
	if(iterMap == m_lpEventMap->end())
	{
		return false;
	}
	else
	{
		EventHandlerList* handlerList = iterMap->second;
		for(EventHandlerList::iterator iterList = handlerList->begin(); iterList != handlerList->end(); ++iterList)
		{
			EventHandler lpCallback = *iterList;
			if(lpCallback->equal(lpHandler))
			{
				handlerList->erase(iterList);
				jgeDelete(lpCallback);

				return true;
			}
		}

		return false;
	}
}

bool JGEEventDispatcher::hasEventListener(int eventID) const
{
	if(m_lpEventMap == null)
	{
		return false;
	}

	EventMap::iterator iterMap = m_lpEventMap->find(eventID);
	return iterMap != m_lpEventMap->end();
}

bool JGEEventDispatcher::dispatchEvent(JGEEvent* lpEvent, bool bubble)
{
	if(lpEvent == null || m_lpEventMap == null)
	{
		return false;
	}

	EventMap::iterator iterMap = m_lpEventMap->find(lpEvent->m_id);
	if(iterMap == m_lpEventMap->end())
	{
		return false;
	}

	EventHandlerList* lpHandlerList = iterMap->second;
	for (EventHandlerList::iterator iterList = lpHandlerList->begin(); iterList != lpHandlerList->end(); ++iterList)
	{
		(*iterList)->invoke(lpEvent);
	}

	if(bubble && m_lpParentBubble != null)
	{
		m_lpParentBubble->dispatchEvent(lpEvent, bubble);
	}

	return true;
}

void JGEEventDispatcher::destroyEventMap()
{
	if(m_lpEventMap != null)
	{
		for (EventMap::iterator iterMap = m_lpEventMap->begin(); iterMap != m_lpEventMap->end(); ++iterMap)
		{
			EventHandlerList* lpHandlerList = iterMap->second;
			for(EventHandlerList::iterator iterList = lpHandlerList->begin(); iterList != lpHandlerList->end(); ++iterList)
			{
				jgeDelete(*iterList);
			}
			jgeDelete(lpHandlerList);
		}
		jgeDelete(m_lpEventMap);
	}
}

void JGEEventDispatcher::removeEventListeners()
{
	destroyEventMap();
}