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
#include "scaler.h"

namespace RollingBall
{
	class RollingBallClass : public EventAcceptable
	{
	private:
		//scaler의 기본 px_rate
		static const int DEFAULT_PX_RATE = 32;

		struct {
			HINSTANCE hInstance;
			HWND hwnd;
		} winAPI;
		BOOL isInitTimer;

		Scaler scaler;
		Paint paint;
		Controller controller;
		vector<Ball> ball;
		//ballSwitch를 삭제하고
		//조작중인 공 인덱스를 0으로 설정하면 좋을 것같다
		int ballSwitch;

		TCHAR buffdrag[3][256];


		//볼스위치가 바뀔때 화살표 그리기
		void paint_BallSwitchArrow();
		void init_scaler(int px_rate);

		void update_window();
		void update_state();
		//드래깅 동작을 수행하는지 추적함
		void trace_dragging(MouseEvent& e);
		//드래깅 동작을 했을때 수행할 동작을 작성
		void dragging_action(PhysicalVector& diff, MouseEvent& e);
		//diff만큼 맵을 이동
		void map_move(PhysicalVector& diff);
		void map_scale(MouseEvent& e);
		
		void ball_add(MouseEvent& e);
		void ball_select(MouseEvent& e);
		//공을 움직였다면 참, 아니면 거짓 반환
		BOOL ball_move(PhysicalVector& diff, MouseEvent& e);
		void set_timer(UINT frame_update_interval);
		void kill_timer();
		/*
		*	이벤트 처리 메서드
		*/
		void event_mouse(MouseEvent e);
		void event_else(Event e);
		void event_keyboard(KeyboardEvent e);

	public:
		RollingBallClass();
		~RollingBallClass();
		//RollingBallClass 변수를 사용하기 전 반드시 수행해야 함
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd, UINT frame_update_interval = 5);
		//윈도우 이벤트를 rollingBall프로그램이 받아들임
		void set_frame_update_interval(UINT millisecond);
	};
}

#endif