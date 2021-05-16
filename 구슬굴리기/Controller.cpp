#include "Controller.h"

using namespace RollingBall;

void Controller::set_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	windowEvent.iMsg = m_iMsg;
	windowEvent.wParam = m_wParam;
	windowEvent.lParam = m_lParam;
}

void Controller::control_keydown()
{
	if (windowEvent.iMsg != WM_KEYDOWN) return;

	switch (windowEvent.wParam)
	{
	case VK_LEFT:
		isPushed.key_left = TRUE;
		break;
	case VK_RIGHT:
		isPushed.key_right = TRUE;
		break;
	case VK_UP:
		isPushed.key_up = TRUE;
		break;
	case VK_DOWN:
		isPushed.key_down = TRUE;
		break;
	}
}

void Controller::control_keyup()
{
	if (windowEvent.iMsg != WM_KEYUP) return;

	switch (windowEvent.wParam)
	{
	case VK_LEFT:
		isPushed.key_left = FALSE;
		break;
	case VK_RIGHT:
		isPushed.key_right = FALSE;
		break;
	case VK_UP:
		isPushed.key_up = FALSE;
		break;
	case VK_DOWN:
		isPushed.key_down = FALSE;
		break;
	}
}

void Controller::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	set_windowEvent(m_iMsg, m_wParam, m_lParam);
	control_keyup();
	control_keydown();
	update_ballPos();
}

void Controller::update_ballPos()
{
	int px = 5;
	if (isPushed.key_left) ballPos.x -= px;
	if (isPushed.key_right) ballPos.x += px;

	if (isPushed.key_up) ballPos.y -= px;
	if (isPushed.key_down) ballPos.y += px;
}
