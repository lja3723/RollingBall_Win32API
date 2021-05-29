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

		struct _m_isPushed{
			struct _m_key {
				static BOOL left;
				static BOOL right;
				static BOOL up;
				static BOOL down;
				static BOOL space;
				static BOOL H;
				static BOOL C;
				static BOOL control;
			} _key;

		} _isPushed;

		void set_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void detect_keyPushed();

	public:
		struct _class_isPushed{
		public:
			struct _class_key{
			public:
				BOOL left();
				BOOL right();
				BOOL up();
				BOOL down();
			} key;
		} isPushed;


		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void update_ballPos(Ball& ball);
		void force_to(Ball& ball, double accel);

		void initialize_ball_data(Ball& ball);
	};
}

#endif