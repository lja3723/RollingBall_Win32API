#include "event.h"
#include <algorithm>

using namespace RollingBall;
using std::find;
using std::sort;

/////////////////////////
// 
//	Event class
// 
/////////////////////////
BOOL RollingBall::Event::isValid()
{
	return m_isValid;
}



/////////////////////////
// 
//	KeyboardEvent class
// 
/////////////////////////
BOOL KeyboardEvent::_state::isInitKeysArray = FALSE;
BOOL KeyboardEvent::_state::keys[numofKeys];

void KeyboardEvent::_state::initKeysArray()
{
	if (!isInitKeysArray)
	{
		for (int i = 0; i < numofKeys; i++)
			keys[i] = FALSE;
		isInitKeysArray = TRUE;
	}
}
void KeyboardEvent::key_down(WPARAM VK_msg)
{
	state.keys[VK_msg] = TRUE;
}
void KeyboardEvent::key_up(WPARAM VK_msg)
{
	state.keys[VK_msg] = FALSE;
}
BOOL KeyboardEvent::isKeyPressed(WPARAM VK_msg)
{
	return state.keys[VK_msg];
}



/////////////////////////
// 
//	EventProducer class
// 
/////////////////////////
MouseEvent EventProducer::produce_mouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent e(iMsg, wParam, lParam);
	switch (iMsg)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_LBUTTONDBLCLK:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_RBUTTONDBLCLK:
		break;
	case WM_MBUTTONDOWN:
		break;
	case WM_MBUTTONUP:
		break;
	case WM_MBUTTONDBLCLK:
		break;
	case WM_MOUSEWHEEL:
		e.scroll = (short)HIWORD(wParam);
		break;
	default:
		e.m_isValid = FALSE;
		break;
	}

	return e;
}
KeyboardEvent EventProducer::produce_keyboardEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	KeyboardEvent e(iMsg, wParam, lParam);

	switch (iMsg)
	{
	case WM_KEYDOWN:
		e.key_down(wParam);
		break;
	case WM_KEYUP:
		e.key_up(wParam);
		break;
	default:
		e.m_isValid = FALSE;
	}

	return e;
}
Event EventProducer::produce_Event(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return Event(iMsg, wParam, lParam);
}
void EventProducer::translate_windowEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent em = produce_mouseEvent(iMsg, wParam, lParam);
	if (em.m_isValid)
	{
		//모든 EventAcceptable 객체에 마우스 이벤트를 보낸다
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			EventAcceptable::object_ref[i]->event_mouse(em);
		return;
	}

	KeyboardEvent ek = produce_keyboardEvent(iMsg, wParam, lParam);
	if (ek.m_isValid)
	{
		//모든 EventAcceptable 객체에 키보드 이벤트를 보낸다
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			EventAcceptable::object_ref[i]->event_keyboard(ek);
		return;
	}

	Event e = produce_Event(iMsg, wParam, lParam);
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			EventAcceptable::object_ref[i]->event_all(e);
}


/////////////////////////////
// 
//	EventAcceptable class
// 
/////////////////////////////
vector<EventAcceptable*> EventAcceptable::object_ref = vector<EventAcceptable*>();

EventAcceptable::EventAcceptable()
{
	object_ref.push_back(this);
}
EventAcceptable::~EventAcceptable()
{
	auto iter = find(object_ref.begin(), object_ref.end(), this);
	if (iter != object_ref.end())
		object_ref.erase(iter);
}