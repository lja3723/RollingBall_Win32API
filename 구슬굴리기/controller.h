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

		class _isPushed
		{
		private:
			class _key
			{
			friend class Controller;
			private:
				BOOL _left;
				BOOL _right;
				BOOL _up;
				BOOL _down;
				BOOL _space;
				BOOL _H;
				BOOL _C;
				BOOL _control;
			public:
				BOOL left();
				BOOL right();
				BOOL up();
				BOOL down();
				BOOL space();
				BOOL H();
				BOOL C();
				BOOL control();
			};

			class _mouse
			{
			friend class Controller;
			private:
				BOOL _lButton;
				BOOL _mButton;
				BOOL _rButton;
			public:
				BOOL lButton();
				BOOL mButton();
				BOOL rButton();
			};

		public:
			_key key;
			_mouse mouse;
		};

		void set_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void detect_keyPushed();

	public:
		_isPushed isPushed;

		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void update_ballPos(Ball& ball);
		void force_to(Ball& ball, double accel);

		void initialize_ball_data(Ball& ball);
	};
}

#endif