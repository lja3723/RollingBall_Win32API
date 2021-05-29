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

void Mouse::init()
{
	for (int i = 0; i < numofButtons; i++)
	{
		buttons[i] = FALSE;
		dragStart[i].x = dragStart[i].y = -1;
		dragEnd[i].x = dragEnd[i].y = -1;
	}
	current.x = current.y = 0;
}

void Mouse::button_down(int btn)
{
	buttons[btn] = TRUE;
	//dragStart
}
void Mouse::button_up(int btn)
{
	buttons[btn] = FALSE;
}

void RollingBall::Mouse::mouse_move(LPARAM lParam)
{
	current.x = LOWORD(lParam);
	current.y = HIWORD(lParam);
	for (int i = 0; i < numofButtons; i++);
}

BOOL Mouse::isLButtonDown()
{
	return buttons[0];
}
BOOL Mouse::isRButtonDown()
{
	return buttons[1];
}
BOOL Mouse::isMButtonDown()
{
	return buttons[2];
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
		button_down(0);
		return;
	case WM_LBUTTONUP:
		button_up(0);
		return;

	case WM_RBUTTONDOWN:
		button_down(1);
		return;
	case WM_RBUTTONUP:
		button_up(1);
		return;

	case WM_MBUTTONDOWN:
		button_down(2);
		return;
	case WM_MBUTTONUP:
		button_up(2);
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


void RollingBall::InputDevice::init()
{
	keyboard.init();
	mouse.init();
}
void RollingBall::InputDevice::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	keyboard.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	mouse.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
