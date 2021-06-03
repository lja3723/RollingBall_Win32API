#include "event.h"
#include <algorithm>

using namespace RollingBall;
using std::find;
using std::sort;


/////////////////////////
// 
//	EventProducer class
// 
/////////////////////////
MouseEvent RollingBall::EventProducer::produce_mouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent e;
	memset(&e, 0, sizeof(e));

	switch (iMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		break;
	}

	return e;
}
KeyboardEvent RollingBall::EventProducer::produce_keyboardEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	KeyboardEvent e;
	switch (iMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		break;
	}

	return e;
}
void RollingBall::EventProducer::translate_windowEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{
			MouseEvent e = produce_mouseEvent(iMsg, wParam, lParam);

			//마우스 좌표에 존재하는 모든 EventAcceptable 객체에 마우스 이벤트를 보낸다
			// -> 나중에 우선순위를 참고해 하나의 객체에 이벤트를 보내게 수정할 것임
			for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
				if (EventAcceptable::object_ref[i]->isObjectArea(e.pos))
					EventAcceptable::object_ref[i]->event_mouse(e);
		}
		return; 

	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			KeyboardEvent e = produce_keyboardEvent(iMsg, wParam, lParam);
			//모든 EventAcceptable 객체에 키보드 이벤트를 보낸다
			for (int i = 0; i < EventAcceptable::object_ref.size(); i++)
				EventAcceptable::object_ref[i]->event_keyboard(e);
		}
		return;
	}
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