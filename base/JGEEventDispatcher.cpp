#include <algorithm>
#include "JGEEventDispatcher.h"
#include "jgeUtil.h"

using namespace std;

JGEEventDispatcher::JGEEventDispatcher()
{
	m_lpEventMap = null;
	m_lpParent = null;
}

JGEEventDispatcher::~JGEEventDispatcher()
{
	if(m_lpEventMap != null)
	{
		for (EventMap::iterator iterMap = m_lpEventMap->begin(); iterMap != m_lpEventMap->end(); ++iterMap)
		{
			EventHandlerList* lpHandlerList = iterMap->second;
			jgeDelete(lpHandlerList);
		}
	}
	jgeDelete(m_lpEventMap);
	m_lpParent = null;
}

bool JGEEventDispatcher::addEventListener(int eventID, EventHandler handler)
{
	if(handler == null)
	{
		return false;
	}

	initEventMap();
	EventMap::iterator iter = m_lpEventMap->find(eventID);
	if(iter == m_lpEventMap->end())
	{
		EventHandlerList* lpHandlerList = null;
		jgeNew(lpHandlerList, EventHandlerList);
		lpHandlerList->push_back(handler);
		(*m_lpEventMap)[eventID] = lpHandlerList;
		return true;
	}
	else
	{
		iter->second->push_back(handler);
		return true;
	}
}

bool JGEEventDispatcher::removeEventListener(int eventID, EventHandler handler)
{
	if(handler == null)
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
		EventHandlerList::iterator iterList = find(handlerList->begin(), handlerList->end(), handler);
		if(iterList == handlerList->end())
		{
			return false;
		}
		else
		{
			handlerList->erase(iterList);
			if(handlerList->empty())
			{
				jgeDelete(handlerList);
				m_lpEventMap->erase(iterMap);
			}
			return true;
		}
	}
}

bool JGEEventDispatcher::hasEventListener(int eventID)
{
	EventMap::iterator iterMap = m_lpEventMap->find(eventID);
	return iterMap != m_lpEventMap->end();
}

bool JGEEventDispatcher::dispatchEvent(JGEEvent* lpEvent, bool bubble)
{
	if(lpEvent == null)
	{
		return false;
	}

	EventMap::iterator iterMap = m_lpEventMap->find(lpEvent->m_id);
	if(iterMap == m_lpEventMap->end())
	{
		return false;
	}
	else
	{
		EventHandlerList* lpHandlerList = iterMap->second;
		for (EventHandlerList::iterator iterList = lpHandlerList->begin(); iterList != lpHandlerList->end(); ++iterList)
		{
			((EventHandler)*iterList)(lpEvent);
		}
		return true;
	}

	if(bubble && m_lpParent != null)
	{
		m_lpParent->dispatchEvent(lpEvent, bubble);
	}
}

void JGEEventDispatcher::initEventMap()
{
	if(m_lpEventMap == null)
	{
		m_lpEventMap = new EventMap();
	}
}