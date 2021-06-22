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

	POINT keyViewer = { 300, 120 };
	int d_keyViewer = 18;

	if (controller.isPushed.key.down())
		paint.text(_T("↓"), keyViewer.x, keyViewer.y);
	if (controller.isPushed.key.up())
		paint.text(_T("↑"), keyViewer.x, keyViewer.y - d_keyViewer);
	if (controller.isPushed.key.left())
		paint.text(_T("←"), keyViewer.x - d_keyViewer, keyViewer.y);
	if (controller.isPushed.key.right())
		paint.text(_T("→"), keyViewer.x + d_keyViewer, keyViewer.y);

	TCHAR buff[256];

	_stprintf_s(buff, 256, _T("ballSwitch:%d"), ballSwitch);
	paint.text(buff, 300, 30);
	_stprintf_s(buff, 256, _T("ball count:%d"), (int)ball.size());
	paint.text(buff, 300, 50);
	PhysicalVector cen = paint.get_scaler().fix_point_physical();
	_stprintf_s(buff, 256, _T("center position:(%3.2f, %3.2f)"), cen.x, cen.y);
	paint.text(buff, 300, 70);

	for (int i = 0; i < ball.size(); i++)
		paint(ball[i]);

	paint.end();
}
void RollingBallClass::update_state()
{
	//static class 멤버변수 ballSwitch를 가림
	//그림자 효과를 위해 가렸음
	static const int ballSwitch = 0;
	PhysicalVector posPrev = ball[ballSwitch].physical.pos;
	for (int i = 0; i < ball.size(); i++) {
		controller.force_to(ball[ballSwitch], 0.015 + 0.001 * ballSwitch);
		controller.update_ballPos(ball[i]);
	}
	PhysicalVector& posNow = ball[ballSwitch].physical.pos;

	//이전 위치에 공을 생성함
	if (posPrev != posNow)
	{
		Ball _ball;
		_ball.physical.pos = posPrev;
		ball.push_back(_ball);
	}

	static const int tail_length = 30;
	if (tail_length < ball.size())
		ball.erase(ball.begin() + 1);
	else if (1 < ball.size() && posPrev == posNow)
		ball.erase(ball.begin() + 1);
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

	_ball.physical.pos(0, 0);
	ball.push_back(_ball);
	//_ball.physical.pos(0, -1);
	//ball.push_back(_ball);

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