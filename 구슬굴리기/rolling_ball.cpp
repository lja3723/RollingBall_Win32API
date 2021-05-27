#include "rolling_ball.h"
#include "debugger.h"

using namespace RollingBall;

BOOL RollingBallClass::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;


	if (!objbmpinfo.Load(winAPI.hwnd, _T("..\\res\\bmp\\object_bmp_info.txt")))
		return FALSE;

	ball.push_back(Ball());

	if (!paint.init(winAPI.hInstance, winAPI.hwnd)) return FALSE;



	//memset(&physics, 0, sizeof(physics));
	return TRUE;
}

void RollingBallClass::update_window()
{
	paint.begin();

	Background background;
	background.physical.size = 25;
	paint(background);


	//Ball ball;
	//ball.physical.pos.x = 500;
	//ball.physical.pos.y = 500;
	ball[0].physical.size = 5;
	paint(ball[0]);

	paint.end();
}

void RollingBallClass::update_state()
{
	controller.update_ballPos(winAPI.hwnd, ball[0]);
}

void RollingBallClass::send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	controller.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	paint.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
