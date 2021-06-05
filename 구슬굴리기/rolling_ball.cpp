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
		paint.info(ball[i], i * 20);

	for (int i = 0; i < ball.size(); i++)
		paint(ball[i]);

	POINT keyViewer = { 300, 100 };
	int d_keyViewer = 18;

	if (controller.isPushed.key.down())
		paint.text(_T("↓"), keyViewer.x, keyViewer.y);
	if (controller.isPushed.key.up())
		paint.text(_T("↑"), keyViewer.x, keyViewer.y - d_keyViewer);
	if (controller.isPushed.key.left())
		paint.text(_T("←"), keyViewer.x - d_keyViewer, keyViewer.y);
	if (controller.isPushed.key.right())
		paint.text(_T("→"), keyViewer.x + d_keyViewer, keyViewer.y);

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

	for (int y = 5; y >= 0; y--)
		for (int x = 0; x <= 4; x++)
		{
			_ball.physical.pos(x, y);
			ball.push_back(_ball);
		}

	//memset(&physics, 0, sizeof(physics));

	return TRUE;
}
void RollingBall::RollingBallClass::set_frame_update_interval(UINT millisecond)
{
	set_timer(millisecond);
}

void RollingBall::RollingBallClass::event_keyboard(KeyboardEvent e)
{
	static BOOL isProcessed = FALSE;
	if (!isProcessed && e.isKeyDown('C'))
	{
		ball[ballSwitch].physical.accel = { 0, 0 };
		ballSwitch++;
		if (ballSwitch == ball.size()) ballSwitch = 0;
		isProcessed = TRUE;
	}
	else if (!e.isKeyDown('C'))
		isProcessed = FALSE;
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