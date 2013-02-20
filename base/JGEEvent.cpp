#include "JGEEvent.h"
#include "jgeUtil.h"

JGEEvent::JGEEvent(int id)
{
	m_id = id;
	m_lpEventDispatcher = null;
}

JGEEvent::~JGEEvent()
{
	m_lpEventDispatcher = null;
}

// 0x eventCategory(0xFF0000) | eventType(0x00FF00) | eventId(0x0000FF)
// jge eventCategory 0x000000
// jge eventMouseType 0x000000
int JGEEvent::MOUSE_DOWN_LEFT = 0x000001;
int JGEEvent::MOUSE_DOWN_RIGHT = 0x000002;
int JGEEvent::MOUSE_DOWN_MIDDLE = 0x000003;
int JGEEvent::MOUSE_UP_LEFT = 0x000004;
int JGEEvent::MOUSE_UP_RIGHT = 0x000005;
int JGEEvent::MOUSE_UP_MIDDLE = 0x000006;
int JGEEvent::MOUSE_CLICK_LEFT = 0x000007;
int JGEEvent::MOUSE_CLICK_RIGHT = 0x000008;
int JGEEvent::MOUSE_CLICK_MIDDLE = 0x000009;
int JGEEvent::MOUSE_OVER = 0x00000A;
int JGEEvent::MOUSE_OUT = 0x00000B;