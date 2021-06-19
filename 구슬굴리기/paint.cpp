#include "paint.h"
#include "debugger.h"
using namespace RollingBall;



/********************************
* 
*		public functions
*
*********************************/
Paint::Paint() 
{
	hInstance = NULL;
	hwnd = NULL;
	memset(&windowRect, 0, sizeof(windowRect));
}
Paint::~Paint()
{
	doubleBuffering_halt();
}
BOOL Paint::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	//초기화 안된 상태에서만 함수실행
	if (isInit()) return TRUE;

	//멤버변수 초기화
	hInstance = m_hInstance;
	hwnd = m_hwnd;
	//hBitmap 초기화
	if (!hBitmap.init(hInstance)) return FALSE;
	//res vector들 초기화
	init_res_vectors(hBitmap.res_count());
	//scaler 초기화
	init_scaler();

	return TRUE;
}
void RollingBall::Paint::set_px_rate(pixel px_rate)
{
	scale.px_rate(px_rate);
}
void RollingBall::Paint::set_fix_point(PhysicalVector fix_point)
{
	scale.fix_point_physical(fix_point);
}
void RollingBall::Paint::set_fix_point(PixelCoord fix_point)
{
	scale.fix_point_pixel(fix_point);
}
void Paint::begin()
{
	hDC.window.mode.set_BeginPaint();
	hDC.window.set(hwnd);
	doubleBuffering_start();
}
void Paint::end()
{
	flush_buffer();
	hDC.window.release(hwnd);
	doubleBuffering_stop();
}
void Paint::background(Object& background)
{
	paint_background_tobuffer(background);
	paint_background_ruller_tobuffer();
}
void RollingBall::Paint::operator()(Object& obj)
{
	paint_tobuffer(obj);
}
void RollingBall::Paint::info(Object& obj, int yPos)
{
	paint_info_tobuffer(obj, yPos);
}
void RollingBall::Paint::text(LPCTSTR text, pixel x, pixel y)
{
	paint_text_tobuffer(text, x, y);
}



/********************************
*
*		private functions
*		- initialization
*
*********************************/
void Paint::init_res_vectors(int res_count)
{
	hBitmap.resize_res_vector(res_count);
	hDC.resize_res_vector(res_count);
}
void RollingBall::Paint::init_scaler()
{
	//프로그램 화면 정중앙과 물리좌표 (0, 0)이 일치하도록 초기화함
	scale.px_rate(DEFAULT.px_rate);
	GetClientRect(hwnd, &windowRect);
	scale.fix_point_pixel(PixelCoord(windowRect.right / 2, windowRect.bottom / 2));
	scale.fix_point_physical(PhysicalVector(0, 0));
}



/********************************
*
*		private functions
*		- BOOL returns
*
*********************************/
BOOL Paint::isInit()
{
	return hInstance != NULL;
}
BOOL Paint::isDoubleBufferingStart()
{
	return flag.isDoubleBufferingStart;
}
BOOL Paint::isReadyToPaint()
{
	return isDoubleBufferingStart();
}
BOOL Paint::isInitDoubleBuffering()
{
	return flag.isInitDoubleBuffering;
}
BOOL Paint::isWindowSizeChanged()
{
	return flag.isWindowSizeChanged;
}



/********************************
*
*		private functions
*		- double buffering manage
*
*********************************/
void Paint::doubleBuffering_init()
{	

	//초기화 안된 상태에서만 함수실행
	if (isInitDoubleBuffering()) return;

	//hDC.mem.set은 한번만 수행해도 됨
	//hDC.mem.set의 의미:
		//hDC.window 호환 DC인 hDC.mem(windowBuffer, res)을 만드는 것
	hDC.mem.set(hDC.window, hDC.mem.windowBuffer);

	//hBitmap.set도 한번만 수행해도 됨
	//hBitmap.set의 의미:
		//화면 DC(hDC.window) 호환이 되며 크기가 windowRect인 windowBuffer의 hBitmap을 만들고
		//hDC.mem에 hBitmap을 선택시키는 것임
	GetClientRect(hwnd, &windowRect);
	hBitmap.set(windowRect, hDC);

	flag.isInitDoubleBuffering = TRUE;
}
void Paint::doubleBuffering_start()
{
	//더블버퍼링 시작 조건을 만족할 때만 함수실행
	if (isDoubleBufferingStart()) return;
	if (!hDC.window.isSet()) return;

	//더블버퍼링 초기 작업이 안되어 있으면 수행함
	if (!isInitDoubleBuffering())
		doubleBuffering_init();

	//윈도우 크기가 변경되었으면 변경된 크기에 맞는 hBitmap.windowBuffer를 새로 설정함
	if (isWindowSizeChanged())
	{
		//hBitmap.windowBuffer를 생성하고 그것을 hDC.mem.windowBuffer에 선택시킨다
		GetClientRect(hwnd, &windowRect);
		hBitmap.windowBuffer.set(windowRect, hDC);

		PixelCoord p(windowRect.right / 2, windowRect.bottom / 2);
		scale.fix_point_pixel(p);

		flag.isWindowSizeChanged = FALSE;
	}

	flag.isDoubleBufferingStart = TRUE;
}
void Paint::doubleBuffering_stop()
{
	if (!isDoubleBufferingStart()) return;
	flag.isDoubleBufferingStart = FALSE;
}
void Paint::doubleBuffering_halt()
{
	//hBitmap을 release한다(hDC.mem.windowBuffer에서 롤백하는 과정 포함)
	//hDC.mem.windowBuffer와 hDC.mem.res를 삭제함
	hBitmap.release(hDC);
	hDC.mem.release();
}



/********************************
*
*		private functions
*		- paint management
*
*********************************/
void Paint::paint_background_tobuffer(Object& background)
{
	if (!isReadyToPaint()) return;

	GetClientRect(hwnd, &windowRect);

	pixel px_size = scale.px(background.physical.size);
	pixel tsize = background.texture_size(scale);
	PixelCoord px_pos = scale.transform(background.physical.pos);
	px_pos.x %= px_size;
	px_pos.y %= px_size;

	SetStretchBltMode(hDC.mem.windowBuffer, COLORONCOLOR);
	for (int i = -1; i * px_size + px_pos.x < windowRect.right; i++)
		for (int j = -1; j * px_size + px_pos.y < windowRect.bottom; j++)
		{
			StretchBlt(
				hDC.mem.windowBuffer,
				i * px_size + px_pos.x, j * px_size + px_pos.y, px_size, px_size,
				hDC.mem.res[hBitmap.bmpidx(background, scale)], 0, 0, tsize, tsize,
				SRCCOPY
			);
		}
}
void Paint::paint_background_ruller_tobuffer()
{
	PhysicalVector p;
	int cm = 40;
	auto winBuff = hDC.mem.windowBuffer;

	//x축, y축 그리기
	p(-cm, 0);
	MoveToEx(winBuff, scale.transform(p).x, scale.transform(p).y, NULL);
	p(cm, 0);
	LineTo(winBuff, scale.transform(p).x, scale.transform(p).y);
	p(0, -cm);
	MoveToEx(winBuff, scale.transform(p).x, scale.transform(p).y, NULL);
	p(0, cm);
	LineTo(winBuff, scale.transform(p).x, scale.transform(p).y);

	//x축 눈금 그리기
	for (int i = -30; i < 30; i++)
	{
		p(i, 0.5);
		MoveToEx(winBuff, scale.transform(p).x, scale.transform(p).y, NULL);
		p(i, -0.5);
		LineTo(winBuff, scale.transform(p).x, scale.transform(p).y);
	}

	//y축 눈금 그리기
	for (int i = -30; i < 30; i++)
	{
		p(0.5, i);
		MoveToEx(winBuff, scale.transform(p).x, scale.transform(p).y, NULL);
		p(-0.5, i);
		LineTo(winBuff, scale.transform(p).x, scale.transform(p).y);
	}
}
void RollingBall::Paint::paint_tobuffer(Object& object)
{
	if (!isReadyToPaint()) return;

	if (_tcscmp(object.bitmap_name(), _T("floor")) == 0)
	{
		paint_background_tobuffer(object);
		paint_background_ruller_tobuffer();
		return;
	}

	else if (_tcscmp(object.bitmap_name(), _T("ball")) == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			BOOL mask;
			DWORD paintmode;
			if (i == 0)
				mask = TRUE, paintmode = SRCAND;
			else if (i == 1)
				mask = FALSE, paintmode = SRCPAINT;

			pixel px_size = scale.px(object.physical.size);
			pixel tsize = object.texture_size(scale);
			PixelCoord px_pos = scale.transform(object.physical.pos);
			SetStretchBltMode(hDC.mem.windowBuffer, COLORONCOLOR);
			StretchBlt(
				hDC.mem.windowBuffer,
				px_pos.x - px_size / 2, px_pos.y - px_size / 2, px_size, px_size,
				hDC.mem.res[hBitmap.bmpidx(object, scale, mask)], 0, 0, tsize, tsize,
				paintmode
			);
		}
	}
}
void RollingBall::Paint::paint_info_tobuffer(Object& object, int yPos)
{
	TCHAR buff[256];
	_stprintf_s(buff, 256, _T("좌표(%lf, %lf)"), object.physical.pos.x, object.physical.pos.y);
	paint_text_tobuffer(buff, 0, yPos);
}
void RollingBall::Paint::paint_text_tobuffer(LPCTSTR text, pixel x, pixel y)
{
	TextOut(hDC.mem.windowBuffer, x, y, text, (int)_tcslen(text));
}
void Paint::flush_buffer()
{
	if (!isReadyToPaint()) return;

	BitBlt(
		hDC.window, 
		0, 0, windowRect.right, windowRect.bottom,
		hDC.mem.windowBuffer, 0, 0,
		SRCCOPY
	);
}
void RollingBall::Paint::event_all(Event e)
{

	if (e.winmsg.iMsg == WM_SIZE)
	{
		switch (e.winmsg.wParam)
		{
		case SIZE_RESTORED:
		case SIZE_MAXIMIZED:
		case SIZE_MAXSHOW:
			flag.isWindowSizeChanged = TRUE;
			break;
		}
	}

	if (e.winmsg.iMsg == WM_TIMER)
	{
		KeyboardEvent ek = e;

		PhysicalVector ppos = scale.fix_point_physical();
		double zoom_in_out_rate = 0.03;
		cm_val move_distance = 0.2;

		if (ek.isKeyDown('O'))
			if (scale.px_rate() > 20)
				scale.px_rate(scale.px_rate() * (1 - zoom_in_out_rate));
		if (ek.isKeyDown('P'))
			if (scale.px_rate() < 720)
				scale.px_rate(scale.px_rate() * (1 + zoom_in_out_rate));

		if (ek.isKeyDown('W'))
			scale.fix_point_physical(ppos(ppos.x, ppos.y + move_distance));
		if (ek.isKeyDown('A'))
			scale.fix_point_physical(ppos(ppos.x - move_distance, ppos.y));
		if (ek.isKeyDown('S'))
			scale.fix_point_physical(ppos(ppos.x, ppos.y - move_distance));
		if (ek.isKeyDown('D'))
			scale.fix_point_physical(ppos(ppos.x + move_distance, ppos.y));
	}
}

