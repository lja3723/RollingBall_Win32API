#include "Controller.h"
#include <tchar.h>

using namespace RollingBall;

void Controller::set_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	windowEvent.iMsg = m_iMsg;
	windowEvent.wParam = m_wParam;
	windowEvent.lParam = m_lParam;
}

void Controller::detect_keyPushed()
{
	BOOL flag_key = FALSE;
	if (windowEvent.iMsg == WM_KEYDOWN)
		flag_key = TRUE;
	else if (windowEvent.iMsg == WM_KEYUP)
		flag_key = FALSE;
	else return;

	switch (windowEvent.wParam)
	{
	case VK_LEFT:
		isPushed.key.left = flag_key;
		break;
	case VK_RIGHT:
		isPushed.key.right = flag_key;
		break;
	case VK_UP:
		isPushed.key.up = flag_key;
		break;
	case VK_DOWN:
		isPushed.key.down = flag_key;
		break;
	case VK_SPACE:
		isPushed.key.space = flag_key;
		break;
	case _T('H'): case _T('h'):
		isPushed.key.H = flag_key;
		break;
	case VK_CONTROL:
		isPushed.key.control = flag_key;
		break;
	}
}

void Controller::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	set_windowEvent(m_iMsg, m_wParam, m_lParam);
	detect_keyPushed();
}

void Controller::update_ballPos(HWND hwnd)
{
	if (isPushed.key.left) x.Speed -= x.Accel;
	if (isPushed.key.right) x.Speed += x.Accel;

	if (isPushed.key.up) y.Speed -= y.Accel;
	if (isPushed.key.down) y.Speed += y.Accel;

	if (isPushed.key.space)
	{
		int xpos = x.Pos, ypos = y.Pos;
		initialize_ball_data();
		x.Pos = xpos, y.Pos = ypos;
	}
	if (isPushed.key.H)
	{
		RECT rt;
		GetClientRect(hwnd, &rt);
		x.Pos = rt.right / 2;
		y.Pos = rt.bottom / 2;
		if (isPushed.key.control)
		{
			initialize_ball_data();
			x.Pos = rt.right / 2;
			y.Pos = rt.bottom / 2;
		}
	}

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
	x.Accel = y.Accel = 0.1;
}

RollingBall::Controller::Controller()
{
	initialize_ball_data();
}
