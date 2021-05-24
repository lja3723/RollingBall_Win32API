#include "rolling_ball.h"

using namespace RollingBall;

BOOL RollingBallClass::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;
	if (!objectManager.init(m_hwnd)) return FALSE;
	if (!paint.init(winAPI.hInstance, winAPI.hwnd)) return FALSE;

	memset(&physics, 0, sizeof(physics));
	return TRUE;
}

void RollingBallClass::update_window()
{
	int x = controller.get_xPos();
	int y = controller.get_yPos();

	paint.beginPaint();

	paint.paint_background();
	paint.paint_ball(x, y, ballsize);

	paint.endPaint();
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
