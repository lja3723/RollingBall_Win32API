#pragma once
#ifndef __Controller_h__
#define __Controller_h__
#include <Windows.h>

namespace RollingBall
{
	class Controller
	{
	public:
		struct {
			int x = 100;
			int y = 100;
		} ballPos;

	private:
		struct {
			UINT iMsg;
			WPARAM wParam;
			LPARAM lParam;
		} windowEvent;

		struct {
			BOOL key_left;
			BOOL key_right;
			BOOL key_up;
			BOOL key_down;

		} isPushed;

		void set_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void control_keydown();
		void control_keyup();
	public:
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		void update_ballPos();
	};
}

#endif