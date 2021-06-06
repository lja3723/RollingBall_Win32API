#include "device_input.h"


using namespace RollingBall;

void RollingBall::Keyboard::init()
{
	for (int i = 0; i < numofKeys; i++)
		keys[i] = FALSE;
}

void Keyboard::key_down(WPARAM VK_msg)
{
	keys[VK_msg] = TRUE;
}
void Keyboard::key_up(WPARAM VK_msg)
{
	keys[VK_msg] = FALSE;
}
BOOL Keyboard::isKeyDown(WPARAM VK_msg)
{
	return keys[VK_msg];
}

void Keyboard::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	switch (m_iMsg)
	{
	case WM_KEYDOWN:
		key_down(m_wParam);
		return;
	case WM_KEYUP:
		key_up(m_wParam);
		return;
	}
}



//https://is03.tistory.com/27

void Mouse::init(HWND m_hwnd)
{
	hwnd = m_hwnd;
	update_mousePos();
	current = { relative.x, relative.y };
	for (int button = LButton; button < numofButtons; button++)
	{
		buttons[button] = FALSE;
		invalidate(&dragStart[button]);
		invalidate(&prevDragging[button]);
		invalidate(&dragging[button]);
		invalidate(&dragEnd[button]);
	}
}

POINT Mouse::delta(POINT* p1, POINT* p2)
{
	POINT p;
	p.x = p2->x - p1->x;
	p.y = p2->y - p1->y;
	return p;
}

void Mouse::update_mousePos()
{
	GetCursorPos(&absolute);
	relative = absolute;
	ScreenToClient(hwnd, &relative);
}

void Mouse::button_down(int btn)
{
	buttons[btn] = TRUE;
	dragStart[btn] = current;

	invalidate(&dragEnd[btn]);
	prevDragging[btn] = current;
	dragging[btn] = current;
}
void Mouse::button_up(int btn)
{
	buttons[btn] = FALSE;
	//dragStart[btn]; 변동없음

	dragEnd[btn] = dragging[btn];
	invalidate(&prevDragging[btn]);
	invalidate(&dragging[btn]);
}

void Mouse::mouse_move(LPARAM lParam)
{
	update_mousePos();
	current = { LOWORD(lParam), HIWORD(lParam) };
	for (int button = 0; button < numofButtons; button++)
		if (buttons[button])
		{
			prevDragging[button] = dragging[button];
			dragging[button] = current;
		}
}

BOOL Mouse::isValid(POINT p)
{
	return p.x != -1 && p.y != -1;
}
void Mouse::invalidate(LPPOINT p)
{
	p->x = p->y = -1;
}

BOOL RollingBall::Mouse::isButtonDown(int btn)
{
	return buttons[btn];
}

BOOL RollingBall::Mouse::isButtonClicked(int btn, POINT* clicked_pos)
{
	BOOL flag = TRUE;
	//버튼이 눌린 상태면 클릭된 것이 아님
	if (buttons[btn]) flag = FALSE;

	//dragStart가 유효하지 않으면 클릭된 것이 아님
	else if (!isValid(dragStart[btn])) flag = FALSE;

	//마우스가 5픽셀 이상 움직였다면 클릭된 것이 아님
	else {
		POINT d = delta(&dragStart[btn], &dragEnd[btn]);
		if (abs(d.x) > 5 || abs(d.y) > 5)
			flag = FALSE;
	}

	//클릭됐으면 클릭된 곳을 드래그 시작한 곳으로 설정
	if (flag)
		*clicked_pos = dragStart[btn];

	//클릭이 안됐으면 클릭 위치를 무효화
	else
		invalidate(clicked_pos);
	
	return flag;
}

BOOL RollingBall::Mouse::isButtonDragging(int btn, POINT* dragStart, POINT* prevDragging)
{
	if (isValid(dragging[btn]))
	return 0;
}

BOOL Mouse::isLButtonDown()
{
	return buttons[LButton];
}
BOOL Mouse::isRButtonDown()
{
	return buttons[RButton];
}
BOOL Mouse::isMButtonDown()
{
	return buttons[MButton];
}

BOOL RollingBall::Mouse::isLButtonClicked()
{


	return 0;
}

BOOL RollingBall::Mouse::isRButtonClicked()
{
	return 0;
}

BOOL RollingBall::Mouse::isMButtonClicked()
{
	return 0;
}

BOOL RollingBall::Mouse::isLButtonDragging()
{
	return 0;
}

BOOL RollingBall::Mouse::isRButtonDragging()
{
	return 0;
}

BOOL RollingBall::Mouse::isMButtonDragging()
{
	return 0;
}

BOOL RollingBall::Mouse::isWheelUp()
{
	return 0;
}

BOOL RollingBall::Mouse::isWheelDown()
{
	return 0;
}

void RollingBall::Mouse::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	switch (m_iMsg)
	{
	case WM_MOUSEMOVE:
		mouse_move(m_lParam);
		return;

	case WM_LBUTTONDOWN:
		button_down(LButton);
		return;
	case WM_LBUTTONUP:
		button_up(LButton);
		return;

	case WM_RBUTTONDOWN:
		button_down(RButton);
		return;
	case WM_RBUTTONUP:
		button_up(RButton);
		return;

	case WM_MBUTTONDOWN:
		button_down(MButton);
		return;
	case WM_MBUTTONUP:
		button_up(MButton);
		return;
	}
}

int RollingBall::Mouse::x()
{
	return 0;
}

int RollingBall::Mouse::y()
{
	return 0;
}


void RollingBall::InputDevice::init(HWND hwnd)
{
	keyboard.init();
	mouse.init(hwnd);
}
void RollingBall::InputDevice::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	keyboard.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	mouse.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
