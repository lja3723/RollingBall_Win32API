#include "rolling_ball.h"
#include "debugger.h"

using namespace RollingBall;

BOOL RollingBallClass::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;

	if (!ObjectBitmapInfoVector::Load(winAPI.hwnd, _T("res\\bmp\\object_bmp_info.txt")))
		return FALSE;

	if (!paint.init(winAPI.hInstance, winAPI.hwnd)) return FALSE;

	Ball _ball;
	_ball.physical.pos(0, 1);
	ball.push_back(_ball);

	_ball.physical.pos(0, -1);
	ball.push_back(_ball);

	//memset(&physics, 0, sizeof(physics));
	return TRUE;
}

void RollingBallClass::update_window()
{
	paint.begin();

	Background background;
	background.physical.size = 10;
	
	paint(background);
	for (int i = 0; i < ball.size(); i++)
		paint.info(ball[i], i * 20),
		paint(ball[i]);

	if (controller.isPushed.key.down())
		paint.text(_T("↓"), 80, 200);
	if (controller.isPushed.key.up())
		paint.text(_T("↑"), 80, 180);
	if (controller.isPushed.key.left())
		paint.text(_T("←"), 60, 200);
	if (controller.isPushed.key.right())
		paint.text(_T("→"), 100, 200);

	paint.end();
}

void RollingBallClass::update_state()
{
	if (!ball.size()) return;

	for (int i = 0; i < ball.size(); i++) {
		controller.force_to(ball[ballSwitch], 0.015 + 0.001*ballSwitch);
		controller.update_ballPos(ball[i]);
	}
}

void RollingBallClass::send_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	switch (m_iMsg)
	{
	case WM_PAINT:
		update_window();
		return;
	case WM_TIMER:
		update_state();
		InvalidateRgn(winAPI.hwnd, NULL, FALSE);
		return;
	default:
		if (m_iMsg == WM_KEYDOWN && m_wParam == _T('C'))
		{
			ballSwitch++;
			if (ballSwitch == ball.size()) ballSwitch = 0;
		}
		controller.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
		paint.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	}
}
