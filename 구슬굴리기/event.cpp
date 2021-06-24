﻿#include "event.h"
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
//	MouseEvent class
// 
/////////////////////////
BOOL MouseEvent::_state::Buttons[numofButtons];
BOOL MouseEvent::_state::isInitButtonsArray = FALSE;

void RollingBall::MouseEvent::_state::initButtonsArray()
{
	if (!isInitButtonsArray)
	{
		for (int i = 0; i < numofButtons; i++)
			Buttons[i] = FALSE;
		isInitButtonsArray = TRUE;
	}
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
MouseEvent EventProducer::produce_mouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent e(iMsg, wParam, lParam);

	if (e.eventType.isLButtonDown())
	{
	}
	else if (e.eventType.isLButtonUp())
	{
	}
	else if (e.eventType.isLButtonDoubleClick())
	{
	}

	else if (e.eventType.isMButtonDown())
	{
	}
	else if (e.eventType.isMButtonUp())
	{
	}
	else if (e.eventType.isMButtonDoubleClick())
	{
	}

	else if (e.eventType.isRButtonDown())
	{
	}
	else if (e.eventType.isRButtonUp())
	{
	}
	else if (e.eventType.isRButtonDoubleClick())
	{
	}

	else if (e.eventType.isMouseMove())
	{
	}
	else if (e.eventType.isMouseWheel())
	{
		e.scroll = (short)HIWORD(wParam);
	}
	else
		e.m_isValid = FALSE;

	return e;
}
KeyboardEvent EventProducer::produce_keyboardEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
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