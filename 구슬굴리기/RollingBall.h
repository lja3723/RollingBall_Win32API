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
		PaintManager paintManager;
		Physics physics;
		Controller controller;

	public:
		void initialize(HINSTANCE hInstance, HWND hwnd);
		void update_window();
		void update_state();
		void send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};
}

#endif
