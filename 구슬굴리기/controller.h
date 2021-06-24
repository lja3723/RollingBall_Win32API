#pragma once
#ifndef __controller_h__
#define __controller_h__
#include <Windows.h>
#include "object.h"
#include "event.h"

namespace RollingBall
{
	class Controller
	{
	public:

		void update_ballPos(Ball& ball);
		void force_to(Ball& ball, double accel);

		void initialize_ball_data(Ball& ball);
	};
}

#endif