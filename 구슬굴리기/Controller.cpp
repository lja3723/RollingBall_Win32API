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
	case VK_SPACE:
		isPushed.key_space = TRUE;
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
	case VK_SPACE:
		isPushed.key_space = FALSE;
		break;
	}
}

void Controller::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	set_windowEvent(m_iMsg, m_wParam, m_lParam);
	control_keyup();
	control_keydown();
}

void Controller::update_ballPos()
{
	if (isPushed.key_left) x.Speed -= x.Accel;
	if (isPushed.key_right) x.Speed += x.Accel;

	if (isPushed.key_up) y.Speed -= y.Accel;
	if (isPushed.key_down) y.Speed += y.Accel;

	if (isPushed.key_space) initialize_ball_data();

	x.Pos += x.Speed;
	y.Pos += y.Speed;
}

int RollingBall::Controller::get_xPos()
{
	return x.Pos;
}
int RollingBall::Controller::get_xSpeed()
{
	return x.Speed;
}
int RollingBall::Controller::get_xAccel()
{
	return x.Accel;
}
int RollingBall::Controller::get_yPos()
{
	return y.Pos;
}
int RollingBall::Controller::get_ySpeed()
{
	return y.Speed;
}
int RollingBall::Controller::get_yAccel()
{
	return y.Accel;
}

void RollingBall::Controller::initialize_ball_data()
{
	x.Pos = y.Pos = 0;
	x.Speed = y.Speed = 0;
	x.Accel = y.Accel = 1;
}

RollingBall::Controller::Controller()
{
	initialize_ball_data();
}
