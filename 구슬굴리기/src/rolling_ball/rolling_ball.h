#pragma once
#ifndef __rolling_ball_h__
#define __rolling_ball_h__

#include <Windows.h>
#include <vector>
#include "../display/paint.h"
#include "../calc/physics.h"
#include "../../controller.h"
#include "../object/object.h"
#include "../event/event.h"

#include "../tool/debugger.h"


namespace RollingBall
{
	class RollingBallClass : public EventAcceptable
	{
	private:
		struct {
			HINSTANCE hInstance;
			HWND hwnd;
		} winAPI;
		BOOL isInitTimer;

		Paint paint;
		Controller controller;
		vector<Ball> ball;

		void update_window();
		void update_state();

		void set_timer(UINT frame_update_interval)
		{
			if (isInitTimer) kill_timer();
			SetTimer(winAPI.hwnd, 1, frame_update_interval, NULL);
			isInitTimer = TRUE;
		}
		void kill_timer() { 
			if (!isInitTimer) return;
			KillTimer(winAPI.hwnd, 1);
			isInitTimer = FALSE;
		}

		int ballSwitch;

		/*
		*	이벤트 처리 메서드
		*/
		virtual void event_all(Event e);
		virtual void event_keyboard(KeyboardEvent e);

	public:
		RollingBallClass() { 
			winAPI = { NULL, NULL }; 
			isInitTimer = FALSE; 
			ballSwitch = 0;
		}
		~RollingBallClass() { 
			if (isInitTimer)
				kill_timer(); 
		}

		//RollingBallClass 변수를 사용하기 전 반드시 수행해야 함
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd, UINT frame_update_interval = 5);
		//윈도우 이벤트를 rollingBall프로그램이 받아들임
		void set_frame_update_interval(UINT millisecond);
	};
}

#endif