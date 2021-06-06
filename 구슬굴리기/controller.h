#pragma once
#ifndef __controller_h__
#define __controller_h__
#include <Windows.h>
#include "src/object/object.h"
#include "src/event/event.h"

namespace RollingBall
{
	class Controller : public EventAcceptable
	{
	private:

		class _isPushed
		{
		private:
			//모든 키를 일일이 하나의 변수로 만드는건 좋은 생각이 아닌듯함
			//VK 매크로를 매개변수로 받는 단일 함수를 만들어야 할듯
			//키 눌림 여부는 배열로 선언해보자
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
				_key();
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
				_mouse();
				BOOL lButton();
				BOOL mButton();
				BOOL rButton();
			};

		public:
			_key key;
			_mouse mouse;
		};

		virtual void event_keyboard(KeyboardEvent e);

	public:
		_isPushed isPushed;

		void update_ballPos(Ball& ball);
		void force_to(Ball& ball, double accel);

		void initialize_ball_data(Ball& ball);
	};
}

#endif