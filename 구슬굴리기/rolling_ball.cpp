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
	paint.begin();

	//배경 그리기
	Background background;
	background.physical.size = 25;
	paint(background);

	//공 정보 그리기
	//for (int i = 0; i < ball.size(); i++)
	for (int i = 0; i < ball.size() && i < 1; i++)
		paint.info(ball[i], i * 20);

	//방향키 뷰어 그리기
	POINT keyViewer = { 300, 120 };
	int d_keyViewer = 18;

	KeyboardEvent e;
	if (e.isKeyPressed(VK_DOWN))
		paint.text(_T("↓"), keyViewer.x, keyViewer.y);
	if (e.isKeyPressed(VK_UP))
		paint.text(_T("↑"), keyViewer.x, keyViewer.y - d_keyViewer);
	if (e.isKeyPressed(VK_LEFT))
		paint.text(_T("←"), keyViewer.x - d_keyViewer, keyViewer.y);
	if (e.isKeyPressed(VK_RIGHT))
		paint.text(_T("→"), keyViewer.x + d_keyViewer, keyViewer.y);

	//기타 정보 그리기
	TCHAR buff[256];
	_stprintf_s(buff, 256, _T("ballSwitch:%d"), ballSwitch);
	//paint.text(buff, 300, 30);
	_stprintf_s(buff, 256, _T("ball count:%d"), (int)ball.size());
	//paint.text(buff, 300, 50);
	PhysicalVector cen = scaler.get_fix_point_physical();
	_stprintf_s(buff, 256, _T("center position:(%3.2f, %3.2f)"), cen.x, cen.y);
	paint.text(buff, 300, 70);

	//mouseEvent 테스트 로직
	if (isMouseEvent)
	{
		paint.text(mouseEventTestBuff[0], 300, 150);
		isMouseEvent = FALSE;
	}
	MouseEvent em;
	if (em.isLButtonDown())
	{
		_stprintf_s(buff, 256, _T("LButton is pressing"));
		paint.text(buff, 300, 170);
	}
	paint.text(mouseEventTestBuff[1], 300, 190);

	//공 그리기
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

	//공 위치가 변하면 이전위치에 공(그림자) 생성
	if (posPrev != posNow)
	{
		Ball _ball;
		_ball.physical.pos = posPrev;
		_ball.physical.size = ball[0].physical.size;
		ball.push_back(_ball);
	}

	//그림자가 길어지면 공 삭제
	static const int tail_length = 30;
	if (tail_length < ball.size())
		ball.erase(ball.begin() + 1);
	else if (1 < ball.size() && posPrev == posNow)
		ball.erase(ball.begin() + 1);
}
void RollingBallClass::update_scaler()
{
	PhysicalVector ppos = scaler.get_fix_point_physical();
	double zoom_in_out_rate = 0.03;
	cm_val move_distance = 0.2;

	KeyboardEvent e;
	if (e.isKeyPressed('O'))
		if (scaler.px_rate() > 20)
			scaler.px_rate(scaler.px_rate() * (1 - zoom_in_out_rate));
	if (e.isKeyPressed('P'))
		if (scaler.px_rate() < 720)
			scaler.px_rate(scaler.px_rate() * (1 + zoom_in_out_rate));

	if (e.isKeyPressed('W'))
		scaler.set_fix_point(ppos(ppos.x, ppos.y + move_distance));
	if (e.isKeyPressed('A'))
		scaler.set_fix_point(ppos(ppos.x - move_distance, ppos.y));
	if (e.isKeyPressed('S'))
		scaler.set_fix_point(ppos(ppos.x, ppos.y - move_distance));
	if (e.isKeyPressed('D'))
		scaler.set_fix_point(ppos(ppos.x + move_distance, ppos.y));
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
	//그림자 효과를 위해 static 클래스 변수 ballSwitch를 가림
	static const int ballSwitch = 0;

	static BOOL isProcessed = FALSE;
	if (!isProcessed && e.isKeyPressed('C'))
	{
		ball[ballSwitch].physical.accel = { 0, 0 };
		//그림자 효과를 위해 주석처리
		//ballSwitch++;
		//if (ballSwitch == ball.size()) ballSwitch = 0;
		isProcessed = TRUE;
	}
	else if (!e.isKeyPressed('C'))
		isProcessed = FALSE;

	if (e.isKeyDown('R'))
		debuggerMessage("You're typed 'R' key.");
}
void RollingBallClass::event_mouse(MouseEvent e)
{
	TCHAR* eventType = (TCHAR*)_T("");
	if (e.eventType.isMouseMove())
		eventType = (TCHAR*)_T("MouseMove");
	else if (e.eventType.isMouseWheelUp())
		eventType = (TCHAR*)_T("MouseWheelUp");
	else if (e.eventType.isMouseWheelDown())
		eventType = (TCHAR*)_T("MouseWheelDown");
	else if (e.eventType.isLButtonDown())
		eventType = (TCHAR*)_T("LButtonDown");
	else if (e.eventType.isMButtonDown())
	{
		//debuggerMessage("MButtonDown");
		eventType = (TCHAR*)_T("MButtonDown");
	}
	else if (e.eventType.isRButtonDown())
		eventType = (TCHAR*)_T("RButtonDown");
	_stprintf_s(mouseEventTestBuff[0], 256, _T("MouseEvent Occurred(%s)"), eventType);

	if (e.eventType.isLButtonDoubleClick())
		debuggerMessage("You are double clicked the LButton.");
	if (e.eventType.isMButtonDoubleClick())
		debuggerMessage("You are double clicked the MButton.");
	if (e.eventType.isRButtonDoubleClick())
		debuggerMessage("You are double clicked the RButton.");

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

	//휠로 맵 확대 축소하기
	if (e.eventType.isMouseWheelUp())
		scaler.px_rate(scaler.px_rate() * 1.1);
	if (e.eventType.isMouseWheelDown())
		scaler.px_rate(scaler.px_rate() * 0.9);

	isMouseEvent = TRUE;
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
		update_scaler();
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

	isMouseEvent = FALSE;
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
