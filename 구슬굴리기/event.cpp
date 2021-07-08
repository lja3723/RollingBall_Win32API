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
BOOL Event::isValid()
{
	return m_isValid;
}
void Event::init()
{
	//wm 멤버가 모두 0이면 FALSE, 그렇지 않으면 TRUE로 초기화
	m_isValid = !!((m_winMsg.hwnd != NULL) | m_winMsg.iMsg | m_winMsg.wParam | m_winMsg.lParam);
}



/////////////////////////
// 
//	MouseEvent class
// 
/////////////////////////
BOOL MouseEvent::_staticState::buttons[numofButtons];
BOOL MouseEvent::_staticState::isInitButtonsArray = FALSE;
void MouseEvent::init()
{
	localState.pos.x = LOWORD(winMsg.lParam());
	localState.pos.y = HIWORD(winMsg.lParam());

	//LButton
	if (eventType.isLButtonDown()) {
		staticState.button_down(staticState.LButton);
		staticState.drag.trace_buttonDown(staticState.LButton, localState.pos.x, localState.pos.y);
	}
	else if (eventType.isLButtonUp()) {
		staticState.button_up(staticState.LButton);
		staticState.drag.trace_buttonUp(staticState.LButton);
	}
	else if (eventType.isLButtonDoubleClick());

	//MButton
	else if (eventType.isMButtonDown()) {
		staticState.button_down(staticState.MButton);
		staticState.drag.trace_buttonDown(staticState.MButton, localState.pos.x, localState.pos.y);
	}
	else if (eventType.isMButtonUp()) {
		staticState.button_up(staticState.MButton);
		staticState.drag.trace_buttonUp(staticState.MButton);
	}
	else if (eventType.isMButtonDoubleClick());

	//RButton
	else if (eventType.isRButtonDown()) {
		staticState.button_down(staticState.RButton);
		staticState.drag.trace_buttonDown(staticState.RButton, localState.pos.x, localState.pos.y);
	}
	else if (eventType.isRButtonUp()) {
		staticState.button_up(staticState.RButton);
		staticState.drag.trace_buttonUp(staticState.RButton);
	}
	else if (eventType.isRButtonDoubleClick());

	//etc
	else if (eventType.isMouseMove()) {
		staticState.drag.trace_mouseMove(localState.pos.x, localState.pos.y);
	}
	else if (eventType.isMouseWheel()) {
		ScreenToClient(winMsg.hwnd(), &localState.pos);
		localState.scroll = (short)HIWORD(winMsg.wParam());
	}
	else
		m_isValid = FALSE;

}
void MouseEvent::_staticState::initButtonsArray()
{
	if (!isInitButtonsArray)
	{
		for (int i = 0; i < numofButtons; i++)
			buttons[i] = FALSE;
		isInitButtonsArray = TRUE;
	}
}
void MouseEvent::_staticState::button_down(int button_idx)
{
	buttons[button_idx] = TRUE;
}
void MouseEvent::_staticState::button_up(int button_idx)
{
	buttons[button_idx] = FALSE;
}


BOOL MouseEvent::_staticState::_drag::isInitDragState = FALSE;
BOOL MouseEvent::_staticState::_drag::m_isDragging[numofButtons];
POINT MouseEvent::_staticState::_drag::startPos[numofButtons];
POINT MouseEvent::_staticState::_drag::prevPos;
POINT MouseEvent::_staticState::_drag::curPos;
void RollingBall::MouseEvent::_staticState::_drag::init()
{
	if (!isInitDragState)
	{
		for (int i = 0; i < numofButtons; i++)
		{
			m_isDragging[i] = FALSE;
			invalidatePos(startPos[i]);
		}
		invalidatePos(prevPos);
		invalidatePos(curPos);
		isInitDragState = TRUE;
	}
}

void RollingBall::MouseEvent::_staticState::_drag::trace_buttonDown(int button_idx, int posX, int posY)
{
	startPos[button_idx] = prevPos = curPos = { posX, posY };
}

void RollingBall::MouseEvent::_staticState::_drag::trace_buttonUp(int button_idx)
{
	m_isDragging[button_idx] = FALSE;
	invalidatePos(startPos[button_idx]);
	invalidatePos(prevPos);
	invalidatePos(curPos);
}

void RollingBall::MouseEvent::_staticState::_drag::trace_mouseMove(int posX, int posY)
{
	prevPos = curPos;
	for (int i = 0; i < numofButtons; i++)
		if (m_isDragging[i])
			curPos = { posX, posY };
		else if (isPosValid(startPos[i]) && isPosValid(prevPos))
			m_isDragging[i] = TRUE;
}


POINT MouseEvent::pos()
{
	return localState.pos;
}
BOOL MouseEvent::isLButtonDown()
{
	return staticState.buttons[staticState.LButton];
}
BOOL MouseEvent::isMButtonDown()
{
	return staticState.buttons[staticState.MButton];
}
BOOL MouseEvent::isRButtonDown()
{
	return staticState.buttons[staticState.RButton];
}



/////////////////////////
// 
//	KeyboardEvent class
// 
/////////////////////////
BOOL KeyboardEvent::_staticState::isInitKeysArray = FALSE;
BOOL KeyboardEvent::_staticState::keys[numofKeys];

void KeyboardEvent::init()
{
	localState.changedKey = winMsg.wParam();
	if (eventType.isKeyDown())
		key_down(localState.changedKey);
	else if (eventType.isKeyUp())
		key_up(localState.changedKey);
	else
		m_isValid = FALSE;

}
void KeyboardEvent::_staticState::initKeysArray()
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
	staticState.keys[VK_msg] = TRUE;
}
void KeyboardEvent::key_up(WPARAM VK_msg)
{
	staticState.keys[VK_msg] = FALSE;
}
BOOL KeyboardEvent::isKeyPressed(WPARAM VK_msg)
{
	return staticState.keys[VK_msg];
}

BOOL KeyboardEvent::isKeyStateChanged(WPARAM VK_msg)
{
	return localState.changedKey == VK_msg;
}
BOOL KeyboardEvent::isKeyDown(WPARAM VK_msg)
{
	return eventType.isKeyDown() && isKeyStateChanged(VK_msg);
}
BOOL KeyboardEvent::isKeyUp(WPARAM VK_msg)
{
	return eventType.isKeyUp() && isKeyStateChanged(VK_msg);
}



/////////////////////////
// 
//	EventSender class
// 
/////////////////////////
void EventSender::translate_windowEvent(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	WindowMessage wm(hwnd, iMsg, wParam, lParam);
	if (MouseEvent(wm).isValid())
	{
		MouseEvent e(wm);
		//모든 EventAcceptable 객체에 마우스 이벤트를 보낸다
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			EventAcceptable::object_ref[i]->event_mouse(e);
		return;
	}

	if (KeyboardEvent(wm).isValid())
	{
		KeyboardEvent e(wm);
		//모든 EventAcceptable 객체에 키보드 이벤트를 보낸다
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			EventAcceptable::object_ref[i]->event_keyboard(e);
		return;
	}

	Event e(wm);
	for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
		EventAcceptable::object_ref[i]->event_else(e);
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