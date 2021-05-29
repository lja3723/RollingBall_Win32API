#include "controller.h"
#include <tchar.h>

using namespace RollingBall;

BOOL Controller::_m_isPushed::_m_key::left = FALSE;
BOOL Controller::_m_isPushed::_m_key::right = FALSE;
BOOL Controller::_m_isPushed::_m_key::up = FALSE;
BOOL Controller::_m_isPushed::_m_key::down = FALSE;
BOOL Controller::_m_isPushed::_m_key::space = FALSE;
BOOL Controller::_m_isPushed::_m_key::H = FALSE;
BOOL Controller::_m_isPushed::_m_key::C = FALSE;
BOOL Controller::_m_isPushed::_m_key::control = FALSE;


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
		_isPushed._key.left = flag_key;
		break;
	case VK_RIGHT:
		_isPushed._key.right = flag_key;
		break;
	case VK_UP:
		_isPushed._key.up = flag_key;
		break;
	case VK_DOWN:
		_isPushed._key.down = flag_key;
		break;
	case VK_SPACE:
		_isPushed._key.space = flag_key;
		break;
	case _T('H'):
		_isPushed._key.H = flag_key;
		break;
	case _T('C'):
		_isPushed._key.C = flag_key;
		break;
	case VK_CONTROL:
		_isPushed._key.control = flag_key;
		break;
	}
}

void Controller::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	set_windowEvent(m_iMsg, m_wParam, m_lParam); 
	detect_keyPushed();
}

void Controller::update_ballPos(Ball& ball)
{
	double fraction = 0.005;

	cm_val& pos_x = ball.physical.pos.x;
	cm_val& pos_y = ball.physical.pos.y;
	cm_val& speed_x = ball.physical.speed.x;
	cm_val& speed_y = ball.physical.speed.y;
	cm_val& accel_x = ball.physical.accel.x;
	cm_val& accel_y = ball.physical.accel.y;

	speed_x += accel_x;
	speed_y += accel_y;

	pos_x += speed_x;
	pos_y += speed_y;


	if (-1 < speed_x && speed_x < 1 &&
		-1 < speed_y && speed_y < 1)
		fraction *= 5;
	speed_x *= (1 - fraction);
	speed_y *= (1 - fraction);

	if (-0.01 < speed_x && speed_x < 0.01)
		speed_x = 0;
	if (-0.01 < speed_y && speed_y < 0.01)
		speed_y = 0;

	/*
	RECT rt;
	double boundConstant = 0.7;
	cm_val mid_ball = ball.physical.size / 2;
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
}

void RollingBall::Controller::force_to(Ball& ball, double accel)
{
	double fraction = 0.005;

	cm_val& pos_x = ball.physical.pos.x;
	cm_val& pos_y = ball.physical.pos.y;
	cm_val& speed_x = ball.physical.speed.x;
	cm_val& speed_y = ball.physical.speed.y;
	cm_val& accel_x = ball.physical.accel.x;
	cm_val& accel_y = ball.physical.accel.y;

	if (_isPushed._key.space)
	{
		fraction = 25 * 0.005;

		speed_x *= (1 - fraction);
		speed_y *= (1 - fraction);
	}
	else if (_isPushed._key.H)
	{
		//RECT rt;
		//GetClientRect(hwnd, &rt);
		pos_x = 0;
		pos_y = 0;
		if (_isPushed._key.control)
		{
			initialize_ball_data(ball);
			pos_x = 0;
			pos_y = 0;
		}
	}
	else
	{
		if (_isPushed._key.left) accel_x = -accel;
		else if (_isPushed._key.right) accel_x = accel;
		else accel_x = 0;

		if (_isPushed._key.up) accel_y = accel;
		else if (_isPushed._key.down) accel_y = -accel;
		else accel_y = 0;
	}
}

void Controller::initialize_ball_data(Ball& ball)
{
	ball.physical.pos.x = 0;
	ball.physical.pos.y = 0;
	ball.physical.speed.x = 0;
	ball.physical.speed.y = 0;
}

BOOL RollingBall::Controller::_class_isPushed::_class_key::left()
{
	Controller* c;
	return c->_isPushed._key.left;
}

BOOL RollingBall::Controller::_class_isPushed::_class_key::right()
{
	Controller* c;
	return c->_isPushed._key.right;
}

BOOL RollingBall::Controller::_class_isPushed::_class_key::up()
{
	Controller* c;
	return c->_isPushed._key.up;
}

BOOL RollingBall::Controller::_class_isPushed::_class_key::down()
{
	Controller* c;
	return c->_isPushed._key.down;
}
