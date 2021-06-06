#include "event.h"
#include <algorithm>
#include "../tool/debugger.h"

using namespace RollingBall;
using std::find;
using std::sort;

/////////////////////////
// 
//	KeyboardEvent class
// 
/////////////////////////
BOOL KeyboardEvent::isInit = FALSE;
BOOL KeyboardEvent::keys[numofKeys];

void KeyboardEvent::key_down(WPARAM VK_msg)
{
	keys[VK_msg] = TRUE;
}
void KeyboardEvent::key_up(WPARAM VK_msg)
{
	keys[VK_msg] = FALSE;
}
void RollingBall::KeyboardEvent::init()
{
	if (!isInit)
	{
		for (int i = 0; i < numofKeys; i++)
			keys[i] = FALSE;
		isInit = TRUE;
	}
}
BOOL KeyboardEvent::isKeyDown(WPARAM VK_msg)
{
	return keys[VK_msg];
}


/////////////////////////
// 
//	EventProducer class
// 
/////////////////////////
MouseEvent RollingBall::EventProducer::produce_mouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
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
		e.isValid = FALSE;
		break;
	}

	return e;
}
KeyboardEvent RollingBall::EventProducer::produce_keyboardEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
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
		e.isValid = FALSE;
	}

	return e;
}
Event RollingBall::EventProducer::produce_Event(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return Event(iMsg, wParam, lParam);
}
void RollingBall::EventProducer::translate_windowEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent em = produce_mouseEvent(iMsg, wParam, lParam);
	if (em.isValid)
	{
		//마우스 좌표에 존재하는 모든 EventAcceptable 객체에 마우스 이벤트를 보낸다
		// -> 나중에 우선순위를 참고해 하나의 객체에 이벤트를 보내게 수정할 것임
		for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
			if (EventAcceptable::object_ref[i]->isObjectArea(em.pos))
				EventAcceptable::object_ref[i]->event_mouse(em);
		return;
	}

	KeyboardEvent ek = produce_keyboardEvent(iMsg, wParam, lParam);
	if (ek.isValid)
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
int EventAcceptable::object_count = 0;
vector<EventAcceptable*> EventAcceptable::object_ref = vector<EventAcceptable*>();

RollingBall::EventAcceptable::EventAcceptable()
{
	object_ref.push_back(this);
}
RollingBall::EventAcceptable::~EventAcceptable()
{
	auto iter = find(object_ref.begin(), object_ref.end(), this);
	if (iter != object_ref.end())
		object_ref.erase(iter);
}