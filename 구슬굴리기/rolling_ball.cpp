#include "rolling_ball.h"
#include "debugger.h"

using namespace RollingBall;

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
		controller.force_to(ball[ballSwitch], 0.015 + 0.001 * ballSwitch);
		controller.update_ballPos(ball[i]);
	}
}

BOOL RollingBallClass::init(HINSTANCE m_hInstance, HWND m_hwnd, UINT frame_update_interval)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;
	set_timer(frame_update_interval);

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
void RollingBall::RollingBallClass::set_frame_update_interval(UINT millisecond)
{
	set_timer(millisecond);
}

void RollingBall::RollingBallClass::event_keyboard(KeyboardEvent e)
{
	if (e.winmsg.iMsg == WM_KEYDOWN && e.winmsg.wParam == 'C') {
		ballSwitch++;
		if (ballSwitch == ball.size()) ballSwitch = 0;
	}
}
void RollingBall::RollingBallClass::event_all(Event e)
{
	static int k = 1;
	switch (e.winmsg.iMsg)
	{
	case WM_PAINT:
		update_window();
		return;
	case WM_TIMER:
		update_state();
		InvalidateRgn(winAPI.hwnd, NULL, FALSE);
		return;
	}
}