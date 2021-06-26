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
void RollingBall::Event::productEventFrom(WindowMessage& wm)
{
	m_isValid = TRUE;
}



/////////////////////////
// 
//	MouseEvent class
// 
/////////////////////////
BOOL MouseEvent::_state::buttons[numofButtons];
BOOL MouseEvent::_state::isInitButtonsArray = FALSE;

void RollingBall::MouseEvent::productEventFrom(WindowMessage& wm)
{
	m_isValid = TRUE;

	state.pos.x = LOWORD(wm.lParam);
	state.pos.y = HIWORD(wm.lParam);

	if (eventType.isLButtonDown())
		state.button_down(state.LButton);
	else if (eventType.isLButtonUp())
		state.button_up(state.LButton);
	else if (eventType.isLButtonDoubleClick());

	else if (eventType.isMButtonDown())
		state.button_down(state.MButton);
	else if (eventType.isMButtonUp())
		state.button_up(state.MButton);
	else if (eventType.isMButtonDoubleClick());

	else if (eventType.isRButtonDown())
		state.button_down(state.RButton);
	else if (eventType.isRButtonUp())
		state.button_up(state.RButton);
	else if (eventType.isRButtonDoubleClick());

	else if (eventType.isMouseMove());
	else if (eventType.isMouseWheel())
		state.scroll = (short)HIWORD(wm.wParam);
	else
		m_isValid = FALSE;

}
void RollingBall::MouseEvent::_state::initButtonsArray()
{
	if (!isInitButtonsArray)
	{
		for (int i = 0; i < numofButtons; i++)
			buttons[i] = FALSE;
		isInitButtonsArray = TRUE;
	}
}

void RollingBall::MouseEvent::_state::button_down(int button_idx)
{
	buttons[button_idx] = TRUE;
}

void RollingBall::MouseEvent::_state::button_up(int button_idx)
{
	buttons[button_idx] = FALSE;
}

POINT RollingBall::MouseEvent::pos()
{
	return state.pos;
}

BOOL RollingBall::MouseEvent::isLButtonDown()
{
	return state.buttons[state.LButton];
}

BOOL RollingBall::MouseEvent::isMButtonDown()
{
	return state.buttons[state.MButton];
}

BOOL RollingBall::MouseEvent::isRButtonDown()
{
	return state.buttons[state.RButton];
}



/////////////////////////
// 
//	KeyboardEvent class
// 
/////////////////////////
BOOL KeyboardEvent::_state::isInitKeysArray = FALSE;
BOOL KeyboardEvent::_state::keys[numofKeys];

void RollingBall::KeyboardEvent::productEventFrom(WindowMessage& wm)
{
	m_isValid = TRUE;

	if (eventType.isKeyDown()) {
		key_down(wm.wParam);
		state.changedKey = wm.wParam;
	}
	else if (eventType.isKeyUp()) {
		key_up(wm.wParam);
		state.changedKey = wm.wParam;
	}
	else
		m_isValid = FALSE;

}
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

BOOL RollingBall::KeyboardEvent::isKeyStateChanged(WPARAM VK_msg)
{
	return state.changedKey == VK_msg;
}

BOOL RollingBall::KeyboardEvent::isKeyDown(WPARAM VK_msg)
{
	return eventType.isKeyDown() && isKeyStateChanged(VK_msg);
}

BOOL RollingBall::KeyboardEvent::isKeyUp(WPARAM VK_msg)
{
	return eventType.isKeyUp() && isKeyStateChanged(VK_msg);
}



/////////////////////////
// 
//	EventProducer class
// 
/////////////////////////
MouseEvent EventProducer::__old_produce_mouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent e(iMsg, wParam, lParam);

	e.state.pos.x = LOWORD(lParam);
	e.state.pos.y = HIWORD(lParam);

	if (e.eventType.isLButtonDown())
		e.state.button_down(e.state.LButton);
	else if (e.eventType.isLButtonUp())
		e.state.button_up(e.state.LButton);
	else if (e.eventType.isLButtonDoubleClick());

	else if (e.eventType.isMButtonDown())
		e.state.button_down(e.state.MButton);
	else if (e.eventType.isMButtonUp())
		e.state.button_up(e.state.MButton);
	else if (e.eventType.isMButtonDoubleClick());

	else if (e.eventType.isRButtonDown())
		e.state.button_down(e.state.RButton);
	else if (e.eventType.isRButtonUp())
		e.state.button_up(e.state.RButton);
	else if (e.eventType.isRButtonDoubleClick());

	else if (e.eventType.isMouseMove());
	else if (e.eventType.isMouseWheel())
		e.state.scroll = (short)HIWORD(wParam);
	else
		e.m_isValid = FALSE;

	return e;
}
KeyboardEvent EventProducer::__old_produce_keyboardEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	KeyboardEvent e(iMsg, wParam, lParam);

	if (e.eventType.isKeyDown()) {
		e.key_down(wParam);
		e.state.changedKey = wParam;
	}
	else if (e.eventType.isKeyUp()) {
		e.key_up(wParam);
		e.state.changedKey = wParam;
	}
	else
		e.m_isValid = FALSE;

	return e;
}
Event EventProducer::__old_produce_Event(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return Event(iMsg, wParam, lParam);
}
void EventProducer::translate_windowEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent em = __old_produce_mouseEvent(iMsg, wParam, lParam);
	if (em.m_isValid)
	{
		//모든 EventAcceptable 객체에 마우스 이벤트를 보낸다
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			EventAcceptable::object_ref[i]->event_mouse(em);
		return;
	}

	KeyboardEvent ek = __old_produce_keyboardEvent(iMsg, wParam, lParam);
	if (ek.m_isValid)
	{
		//모든 EventAcceptable 객체에 키보드 이벤트를 보낸다
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			EventAcceptable::object_ref[i]->event_keyboard(ek);
		return;
	}

	Event e = __old_produce_Event(iMsg, wParam, lParam);
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
