#pragma once
#ifndef __controller_h__
#define __controller_h__
#include <Windows.h>
#include "object.h"

namespace RollingBall
{
	class Controller
	{
	private:
		struct {
			UINT iMsg;
			WPARAM wParam;
			LPARAM lParam;
		} windowEvent;

		struct {
			struct {
				BOOL left;
				BOOL right;
				BOOL up;
				BOOL down;
				BOOL space;
				BOOL H;
				BOOL C;
				BOOL control;
			} key;

		} isPushed;

		void set_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void detect_keyPushed();

	public:
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void update_ballPos(Ball& ball);
		void force_to(Ball& ball, double accel);

		void initialize_ball_data(Ball& ball);
	};
}

#endif