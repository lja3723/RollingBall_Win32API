#pragma once
#ifndef __rolling_ball_h__
#define __rolling_ball_h__

#include <Windows.h>
#include "paint.h"
#include "physics.h"
#include "controller.h"
#include "object.h"

namespace RollingBall
{
	class RollingBallClass
	{
	private:
		struct {
			HINSTANCE hInstance;
			HWND hwnd;
		} winAPI;

		Paint paint;
		Physics physics;
		Controller controller;
		Object objectManager;

	public:
		//RollingBallClass 변수를 사용하기 전 반드시 수행해야 함
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd);
		const static int ballsize = 64;

		void update_window();
		void update_state();
		void send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};
}

#endif
