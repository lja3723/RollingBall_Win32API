#include "controller.h"
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

void Controller::update_ballPos(HWND hwnd, Ball& ball)
{
	double fraction = 0.005;

	cm_val& pos_x = ball.physical.pos.x;
	cm_val& pos_y = ball.physical.pos.y;
	cm_val& speed_x = ball.physical.speed.x;
	cm_val& speed_y = ball.physical.speed.y;
	cm_val& accel_x = ball.physical.accel.x;
	cm_val& accel_y = ball.physical.accel.y;

	if (isPushed.key.space)
	{
		fraction *= 25;
	}
	else if (isPushed.key.H)
	{
		//RECT rt;
		//GetClientRect(hwnd, &rt);
		pos_x = 0;
		pos_y = 0;
		if (isPushed.key.control)
		{
			initialize_ball_data(ball);
			pos_x = 1;
			pos_y = 1;
		}
	}
	else
	{

		if (isPushed.key.left) speed_x -= accel_x;
		if (isPushed.key.right) speed_y += accel_x;

		if (isPushed.key.up) speed_y -= accel_y;
		if (isPushed.key.down) speed_y += accel_y;
	}	

	pos_x += speed_x;
	pos_y += speed_y;

	RECT rt;
	GetClientRect(hwnd, &rt);
	double boundConstant = 0.7;
	cm_val mid_ball = ball.physical.size / 2;
	/*
	if (mid_ball > pos_x || pos_x > (int)rt.right - mid_ball)
	{
		speed_x = -boundConstant * speed_x;
		if (mid_ball > pos_x)
			pos_x = mid_ball;
		else
			pos_x = (int)rt.right - mid_ball;
	}
	if (mid_ball > pos_y || pos_y > (int)rt.bottom - mid_ball)
	{
		speed_y = -boundConstant * speed_y;
		if (mid_ball > pos_y)
			pos_y = mid_ball;
		else
			pos_y = rt.bottom - mid_ball;
	}
	*/

	if (-1 < speed_x && speed_x < 1 &&
		-1 < speed_y && speed_y < 1)
		fraction *= 5;
	speed_x *= (1 - fraction);
	speed_y *= (1 - fraction);

	if (-0.01 < speed_x && speed_x < 0.01)
		speed_x = 0;
	if (-0.01 < speed_y && speed_y < 0.01)
		speed_y = 0;
}

void Controller::initialize_ball_data(Ball& ball)
{
	ball.physical.pos.x = 0;
	ball.physical.pos.y = 0;
	ball.physical.speed.x = 0;
	ball.physical.speed.y = 0;
	ball.physical.accel.x = 1;
	ball.physical.accel.y = 1;
}