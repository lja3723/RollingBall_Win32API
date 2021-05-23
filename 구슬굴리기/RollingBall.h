#pragma once
#ifndef __RollingBall_h__
#define __RollingBall_h__

#include <Windows.h>
#include "PaintManager.h"
#include "Physics.h"
#include "Controller.h"

namespace RollingBall
{
	class RollingBallClass
	{
	private:
		struct {
			HINSTANCE hInstance;
			HWND hwnd;
		} winAPI;

		PaintManager paintManager;
		Physics physics;
		Controller controller;

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
