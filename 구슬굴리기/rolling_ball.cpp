#include "rolling_ball.h"
#include "debugger.h"

using namespace RollingBall;

BOOL RollingBallClass::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;

	if (!objbmpinfo.Load(winAPI.hwnd, _T("..\\res\\bmp\\object_bmp_info.txt")))
		return FALSE;

	if (!paint.init(winAPI.hInstance, winAPI.hwnd)) return FALSE;
	Ball _ball;
	_ball.physical.pos.x = 0;
	_ball.physical.pos.y = 0;
	ball.push_back(_ball);

	//memset(&physics, 0, sizeof(physics));
	return TRUE;
}

void RollingBall::RollingBallClass::add_ball(Ball m_ball)
{
	ball.push_back(m_ball);
}

void RollingBallClass::update_window()
{
	paint.begin();

	Background background;
	background.physical.size = 25;
	paint(background);

	static int t = 0;
	/*
	if (t == 0)
	{
		debuggerMessage("%f, %f", ball[0].physical.pos.x, ball[0].physical.pos.y);
		t++;
	}
	*/
	//Ball ball;
	//ball.physical.pos.x = 500;
	//ball.physical.pos.y = 500;
	//paint(ball[0]);
	paint(ball[1]);

	paint.end();
}

void RollingBallClass::update_state()
{
	if (ball.size() > 1)
		controller.update_ballPos(winAPI.hwnd, ball[1]);
}

void RollingBallClass::send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	controller.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	paint.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
