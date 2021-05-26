#include "rolling_ball.h"

using namespace RollingBall;

BOOL RollingBallClass::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;
	if (!objbmpinfo.Load(winAPI.hwnd, _T("..\\res\\bmp\\object_bmp_info.txt")))
		return FALSE;
	if (!paint.init(winAPI.hInstance, winAPI.hwnd)) return FALSE;

	//memset(&physics, 0, sizeof(physics));
	return TRUE;
}

void RollingBallClass::update_window()
{
	Background background;
	Ball ball;

	ball.physical.pos.x = controller.get_xPos();
	ball.physical.pos.y = controller.get_yPos();
	ball.physical.size = 90;
	paint.begin();

	paint(background);
	paint(ball);

	paint.end();
}

void RollingBallClass::update_state()
{
	controller.update_ballPos(winAPI.hwnd, ballsize);
}

void RollingBallClass::send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	controller.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	paint.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
