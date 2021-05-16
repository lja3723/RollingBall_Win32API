#include "RollingBall.h"

using namespace RollingBall;

void RollingBallClass::initialize(HINSTANCE hInstance, HWND hwnd)
{
	printManager.initialize(hInstance, hwnd);
}

void RollingBallClass::update_window()
{
	int x = controller.ballPos.x;
	int y = controller.ballPos.y;
	printManager.set_hDC();

	printManager.draw_background_tobuffer(NULL);
	printManager.draw_ball_tobuffer(x, y);

	printManager.flush_buffer();
	printManager.release_hDC();
}

void RollingBallClass::update_state()
{
	controller.update_ballPos();
}

void RollingBallClass::send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	controller.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
