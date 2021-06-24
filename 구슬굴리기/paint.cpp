#include "paint.h"

using namespace RollingBall;

/********************************
* 
*		public functions
*
*********************************/
BOOL Paint::init(HINSTANCE m_hInstance, HWND m_hwnd, Scaler* _scaler)
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
	//scaler 등록
	register_scaler(_scaler);

	return TRUE;
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
void Paint::background(RollingBallObject& background)
{
	paint_background_tobuffer(background);
	paint_background_ruller_tobuffer();
}
void Paint::operator()(RollingBallObject& obj)
{
	paint_tobuffer(obj);
}
void Paint::info(RollingBallObject& obj, int yPos)
{
	paint_info_tobuffer(obj, yPos);
}
void Paint::text(LPCTSTR text, pixel x, pixel y)
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
void Paint::register_scaler(Scaler* _scaler)
{
	scaler = _scaler;
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
	hBitmap.windowBuffer.set(windowRect, hDC.window);
	hDC.mem.windowBuffer.select(hBitmap);

	hBitmap.res.set();
	hDC.mem.res.select(hBitmap);

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
		hBitmap.windowBuffer.set(windowRect, hDC.window);
		hDC.mem.windowBuffer.select(hBitmap);

		PixelCoord p(windowRect.right / 2, windowRect.bottom / 2);
		scaler->set_fix_point(p);

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
	hDC.mem.windowBuffer.restore(hBitmap);
	hBitmap.windowBuffer.release();
	hDC.mem.res.restore(hBitmap);
	hBitmap.res.release();

	//hDC.mem.windowBuffer와 hDC.mem.res를 삭제함
	hDC.mem.release();
}



/********************************
*
*		private functions
*		- paint management
*
*********************************/
void Paint::paint_background_tobuffer(RollingBallObject& background)
{
	if (!isReadyToPaint()) return;

	GetClientRect(hwnd, &windowRect);

	pixel px_size = scaler->px(background.physical.size);
	pixel tsize = background.texture_size(*scaler);
	PixelCoord px_pos = scaler->transform(background.physical.pos);
	px_pos.x %= px_size;
	px_pos.y %= px_size;

	SetStretchBltMode(hDC.mem.windowBuffer, COLORONCOLOR);
	for (int i = -1; i * px_size + px_pos.x < windowRect.right; i++)
		for (int j = -1; j * px_size + px_pos.y < windowRect.bottom; j++)
		{
			StretchBlt(
				hDC.mem.windowBuffer,
				i * px_size + px_pos.x, j * px_size + px_pos.y, px_size, px_size,
				hDC.mem.res(hBitmap.bmpidx(background, *scaler)), 0, 0, tsize, tsize,
				SRCCOPY
			);
		}
}
void Paint::paint_background_ruller_tobuffer()
{
	PhysicalVector p;
	int cm = 40;
	auto winBuff = hDC.mem.windowBuffer;

	//물리좌표계를 픽셀좌표로 쉽게 변환시키는 도우미 클래스
	class _t{
	private:
		Scaler* scaler;
	public:
		_t(Scaler* _s) { scaler = _s; }
		PixelCoord operator()(PhysicalVector& v) { return scaler->transform(v); }
	} t(scaler);

	//x축, y축 그리기
	p(-cm, 0);
	MoveToEx(winBuff, t(p).x, t(p).y, NULL);
	p(cm, 0);
	LineTo(winBuff, t(p).x, t(p).y);
	p(0, -cm);
	MoveToEx(winBuff, t(p).x, t(p).y, NULL);
	p(0, cm);
	LineTo(winBuff, t(p).x, t(p).y);

	//x축 눈금 그리기
	for (int i = -30; i < 30; i++)
	{
		p(i, 0.5);
		MoveToEx(winBuff, t(p).x, t(p).y, NULL);
		p(i, -0.5);
		LineTo(winBuff, t(p).x, t(p).y);
	}

	//y축 눈금 그리기
	for (int i = -30; i < 30; i++)
	{
		p(0.5, i);
		MoveToEx(winBuff, t(p).x, t(p).y, NULL);
		p(-0.5, i);
		LineTo(winBuff, t(p).x, t(p).y);
	}
}
void Paint::paint_tobuffer(RollingBallObject& object)
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

			pixel px_size = scaler->px(object.physical.size);
			pixel tsize = object.texture_size(*scaler);
			PixelCoord px_pos = scaler->transform(object.physical.pos);
			SetStretchBltMode(hDC.mem.windowBuffer, COLORONCOLOR);
			StretchBlt(
				hDC.mem.windowBuffer,
				px_pos.x - px_size / 2, px_pos.y - px_size / 2, px_size, px_size,
				hDC.mem.res(hBitmap.bmpidx(object, *scaler, mask)), 0, 0, tsize, tsize,
				paintmode
			);
		}
	}
}
void Paint::paint_info_tobuffer(RollingBallObject& object, int yPos)
{
	TCHAR buff[256];
	_stprintf_s(buff, 256, _T("좌표(%lf, %lf)"), object.physical.pos.x, object.physical.pos.y);
	paint_text_tobuffer(buff, 0, yPos);
}
void Paint::paint_text_tobuffer(LPCTSTR text, pixel x, pixel y)
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



/********************************
*
*		private functions
*		- event processing
*
*********************************/
void Paint::event_all(Event e)
{
	if (e.isWinMsg.iMsg(WM_SIZE))
		if 
		(e.isWinMsg.wParam(SIZE_RESTORED)	||
		e.isWinMsg.wParam(SIZE_MAXIMIZED)	||
		e.isWinMsg.wParam(SIZE_MAXSHOW))
			flag.isWindowSizeChanged = TRUE;
}

