#pragma once
#ifndef __rolling_ball_h__
#define __rolling_ball_h__

#include <Windows.h>
#include <vector>
#include "paint.h"
#include "physics.h"
#include "controller.h"
#include "object.h"
#include "event.h"


namespace RollingBall
{
	class RollingBallClass : public EventAcceptable
	{
	private:
		struct {
			HINSTANCE hInstance;
			HWND hwnd;
		} winAPI;

		Monitor paint;
		Controller controller;
		vector<Ball> ball;

		void update_window();
		void update_state();

		int ballSwitch;
	public:
		//RollingBallClass 변수를 사용하기 전 반드시 수행해야 함
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd);

		//윈도우 이벤트를 rollingBall프로그램이 받아들임
		void send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);

		void event_all(Event e);
		void event_keyboard(KeyboardEvent e);
	};
}

#endif