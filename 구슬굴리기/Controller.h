#pragma once
#ifndef __Controller_h__
#define __Controller_h__
#include <Windows.h>

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
				BOOL control;
			} key;

		} isPushed;

		struct {
			int Pos;
			int Speed;
			int Accel;
		} x;

		struct {
			int Pos;
			int Speed;
			int Accel;
		} y;

		void set_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void detect_keyPushed();


	public:
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void update_ballPos(HWND hwnd);

		int get_xPos();
		int get_xSpeed();
		int get_xAccel();
		int get_yPos();
		int get_ySpeed();
		int get_yAccel();

		void initialize_ball_data();

		Controller();
	};
}

#endif