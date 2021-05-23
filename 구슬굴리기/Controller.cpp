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

void Controller::update_ballPos(HWND hwnd, int ballsize)
{
	double fraction = 0.005;

	if (isPushed.key.space)
	{
		fraction *= 25;
	}
	else if (isPushed.key.H)
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
	else
	{

		if (isPushed.key.left) x.Speed -= x.Accel;
		if (isPushed.key.right) x.Speed += x.Accel;

		if (isPushed.key.up) y.Speed -= y.Accel;
		if (isPushed.key.down) y.Speed += y.Accel;
	}	

	x.Pos += x.Speed;
	y.Pos += y.Speed;

	RECT rt;
	GetClientRect(hwnd, &rt);
	double boundConstant = 0.7;
	int mid_ball = ballsize / 2;
	if (mid_ball > x.Pos || x.Pos > (int)rt.right - mid_ball)
	{
		x.Speed = -boundConstant *x.Speed;
		if (mid_ball > x.Pos)
			x.Pos = mid_ball;
		else
			x.Pos = (int)rt.right - mid_ball;
	}
	if (mid_ball > y.Pos || y.Pos > (int)rt.bottom - mid_ball)
	{
		y.Speed = -boundConstant *y.Speed;
		if (mid_ball > y.Pos)
			y.Pos = mid_ball;
		else
			y.Pos = rt.bottom - mid_ball;
	}

	if (-1 < x.Speed && x.Speed < 1 &&
		-1 < y.Speed && y.Speed < 1)
		fraction *= 5;
	x.Speed *= (1 - fraction);
	y.Speed *= (1 - fraction);

	if (-0.01 < x.Speed && x.Speed < 0.01)
		x.Speed = 0;
	if (-0.01 < y.Speed && y.Speed < 0.01)
		y.Speed = 0;
}

int RollingBall::Controller::get_xPos()
{
	return (int)x.Pos;
}
int RollingBall::Controller::get_xSpeed()
{
	return (int)x.Speed;
}
int RollingBall::Controller::get_xAccel()
{
	return (int)x.Accel;
}
int RollingBall::Controller::get_yPos()
{
	return (int)y.Pos;
}
int RollingBall::Controller::get_ySpeed()
{
	return (int)y.Speed;
}
int RollingBall::Controller::get_yAccel()
{
	return (int)y.Accel;
}

void RollingBall::Controller::initialize_ball_data()
{
	x.Pos = y.Pos = 150;
	x.Speed = y.Speed = 0;
	x.Accel = y.Accel = 0.3;
}

RollingBall::Controller::Controller()
{
	initialize_ball_data();
}
