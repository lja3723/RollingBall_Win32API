#include "RollingBall.h"

using namespace RollingBall;

void RollingBallClass::initialize(HINSTANCE m_hInstance, HWND m_hwnd)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;
	paintManager.initialize(winAPI.hInstance, winAPI.hwnd);
}

void RollingBallClass::update_window()
{
	int x = controller.get_xPos() / 10;
	int y = controller.get_yPos() / 10;

	paintManager.beginPaint();

	paintManager.paint_background();
	paintManager.paint_ball(x, y);

	paintManager.endPaint();
}

void RollingBallClass::update_state()
{
	controller.update_ballPos(winAPI.hwnd);
}

void RollingBallClass::send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	controller.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
