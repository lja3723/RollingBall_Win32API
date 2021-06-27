#include "controller.h"
#include <tchar.h>
#include <cmath>

using namespace RollingBall;

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

	auto speed = sqrt(pow(speed_x, 2) + pow(speed_y, 2));

	if (-pow(10, -3) < speed && speed < pow(10, -3))
	{
		speed_x = 0;
		speed_y = 0;
	}

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

	KeyboardEvent e;
	if (e.isKeyPressed('B'))
	{
		fraction = 25 * 0.005;

		speed_x *= (1 - fraction);
		speed_y *= (1 - fraction);
	}
	else if (e.isKeyPressed('H'))
	{
		//RECT rt;
		//GetClientRect(hwnd, &rt);
		pos_x = 0;
		pos_y = 0;
		if (e.isKeyPressed(VK_CONTROL))
		{
			initialize_ball_data(ball);
			pos_x = 0;
			pos_y = 0;
		}
	}
	else
	{
		if (e.isKeyPressed('A')) accel_x = -accel;
		else if (e.isKeyPressed('D')) accel_x = accel;
		else accel_x = 0;

		if (e.isKeyPressed('W')) accel_y = accel;
		else if (e.isKeyPressed('S')) accel_y = -accel;
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