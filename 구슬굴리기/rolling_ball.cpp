#include "rolling_ball.h"
#include "debugger.h"

using namespace RollingBall;

void RollingBallClass::init_scaler(int px_rate)
{
	//프로그램 화면 정중앙과 물리좌표 (0, 0)이 일치하도록 초기화함
	scaler.px_rate(px_rate);

	RECT windowRect;
	GetClientRect(winAPI.hwnd, &windowRect);
	scaler.set_fix_point(PixelCoord(windowRect.right / 2, windowRect.bottom / 2));
	scaler.set_fix_point(PhysicalVector(0, 0));
}

void RollingBallClass::update_window()
{
	paint.setModeBeginPaint();
	paint.begin();

	//배경 그리기
	Background background;
	background.physical.size = 25;
	paint(background);

	//공 정보 그리기
	for (int i = 0; i < ball.size(); i++)
		paint.info(ball[i], i * 20);

	//방향키 뷰어 그리기
	POINT keyViewer = { 300, 120 };
	int d_keyViewer = 18;

	KeyboardEvent e;
	if (e.isKeyPressed('S'))
		paint.text(_T("S"), keyViewer.x, keyViewer.y);
	if (e.isKeyPressed('W'))
		paint.text(_T("W"), keyViewer.x, keyViewer.y - d_keyViewer);
	if (e.isKeyPressed('A'))
		paint.text(_T("A"), keyViewer.x - d_keyViewer, keyViewer.y);
	if (e.isKeyPressed('D'))
		paint.text(_T("D"), keyViewer.x + d_keyViewer, keyViewer.y);

	//기타 정보 그리기
	TCHAR buff[256];
	_stprintf_s(buff, 256, _T("ballSwitch:%d"), ballSwitch);
	paint.text(buff, 300, 30);
	_stprintf_s(buff, 256, _T("ball count:%d"), (int)ball.size());
	paint.text(buff, 300, 50);
	PhysicalVector cen = scaler.get_fix_point_physical();
	_stprintf_s(buff, 256, _T("center position:(%3.2f, %3.2f)"), cen.x, cen.y);
	paint.text(buff, 300, 70);

	//공 그리기
	for (int i = 0; i < ball.size(); i++)
		paint(ball[i]);

	paint.end();
}
void RollingBallClass::update_state()
{
	PhysicalVector posPrev = ball[ballSwitch].physical.pos;
	for (int i = 0; i < ball.size(); i++) {
		controller.force_to(ball[ballSwitch], 0.015 + 0.001 * ballSwitch);
		controller.update_ballPos(ball[i]);
	}
	PhysicalVector& posNow = ball[ballSwitch].physical.pos;
}
void RollingBallClass::update_scaler(MouseEvent& e)
{
	//드래그로 맵 이동하기
	if (e.isLButtonDown())
	{
		static PixelCoord prevPos;
		if (!e.eventType.isMouseMove())
			prevPos = e.pos();
		else
		{
			PixelCoord curPos(e.pos());
			PhysicalVector diff = scaler.transform(curPos) - scaler.transform(prevPos);
			scaler.set_fix_point(scaler.get_fix_point_physical() - diff);
			prevPos = curPos;
		}
	}

	//휠조작으로 맵 확대율 변경하기
	double zoom_rate = 1.1;
	if (e.eventType.isMouseWheelUp())
		if (scaler.px_rate() < 500)
			scaler.px_rate(scaler.px_rate() * zoom_rate);
	if (e.eventType.isMouseWheelDown())
		if (scaler.px_rate() > 5)
			scaler.px_rate(scaler.px_rate() / zoom_rate);
}

void RollingBallClass::set_timer(UINT frame_update_interval)
{
	if (isInitTimer) kill_timer();
	SetTimer(winAPI.hwnd, 1, frame_update_interval, NULL);
	isInitTimer = TRUE;
}
void RollingBallClass::kill_timer()
{
	if (!isInitTimer) return;
	KillTimer(winAPI.hwnd, 1);
	isInitTimer = FALSE;
}


/////////////////////////////////
//	event processing
/////////////////////////////////
void RollingBallClass::event_keyboard(KeyboardEvent e)
{
	if (e.isKeyPressed(VK_SPACE))
	{
		ball[ballSwitch].physical.accel = { 0, 0 };
		ballSwitch++;
		if (ballSwitch == ball.size()) ballSwitch = 0;
	}
}
void RollingBallClass::event_mouse(MouseEvent e)
{
	update_scaler(e);
	if (e.eventType.isLButtonDoubleClick())
	{
		Ball b;
		b.physical.pos(scaler.transform(e.pos()));
		ball.push_back(b);
	}
}
void RollingBallClass::event_else(Event e)
{
	switch (e.winMsg.iMsg())
	{
	case WM_PAINT:
		update_window();
		break;
	case WM_TIMER:
		update_state();
		InvalidateRgn(winAPI.hwnd, NULL, FALSE);
		break;
	}
}


///////////////////////////////////
//	public interface
///////////////////////////////////
RollingBallClass::RollingBallClass()
{
	winAPI = { NULL, NULL };
	isInitTimer = FALSE;
	ballSwitch = 0;
}
RollingBallClass::~RollingBallClass()
{
	if (isInitTimer)
		kill_timer();
}
BOOL RollingBallClass::init(HINSTANCE m_hInstance, HWND m_hwnd, UINT frame_update_interval)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;
	set_timer(frame_update_interval);
	init_scaler(DEFAULT_PX_RATE);

	if (!ObjectBitmapInfoVector::Load(winAPI.hwnd, _T("res\\bmp\\object_bmp_info.txt")))
		return FALSE;

	if (!paint.init(winAPI.hInstance, winAPI.hwnd, &scaler)) return FALSE;

	Ball _ball;
	_ball.physical.size = 2;
	ball.push_back(_ball);

	return TRUE;
}
void RollingBallClass::set_frame_update_interval(UINT millisecond)
{
	set_timer(millisecond);
}
