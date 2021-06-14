#include "paint.h"
#include "debugger.h"
using namespace RollingBall;



/********************************
*
*		static variables
*		initialization
*
*********************************/
int Paint::res_count = 0;


/********************************
* 
*		public functions
*
*********************************/
Paint::~Paint()
{
	doubleBuffering_halt();
}
BOOL Paint::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	if (isInit()) return TRUE;

	hInstance = m_hInstance;
	hwnd = m_hwnd;
	if (!bmp.init(hInstance)) return FALSE;

	init_flags();
	init_res_count();
	scale_set(32);

	memset(&windowRect, 0, sizeof(windowRect));

	hBitmap_windowBuffer_init();
	hBitmap_res_init();

	hBitmap_old_windowBuffer_init();
	hBitmap_old_res_init();

	hBitmap_res_set();

	return TRUE;
}
void RollingBall::Paint::scale_set(pixel px_rate)
{
	scale.px_rate(px_rate);
	scale.fix_point_physical(PhysicalVector(5, 5));
	GetClientRect(hwnd, &windowRect);
	scale.fix_point_pixel(PixelCoord(windowRect.right / 2, windowRect.bottom / 2));
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
//처음 init 될 때 한 번만 호출되는 함수
void Paint::init_flags()
{
	hDC.window.mode.set_BeginPaint();

	flag.isSetHBitmapRes = FALSE;
	flag.isBackedUpHBitmapRes = FALSE;

	flag.isDoubleBufferingStart = FALSE;
	flag.isInitDoubleBuffering = FALSE;
	flag.isWindowSizeChanged = FALSE;
}
void Paint::init_res_count()
{
	if (!bmp.isInit()) return;
	res_count = bmp.file_count();
	init_res_vectors();
}
void Paint::init_res_vectors()
{
	hDC.mem.res.resize(res_count);
	hBitmap.res.resize(res_count);
	hBitmap.old.res.resize(res_count);
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


BOOL Paint::isSetHBitmapWindowBuffer()
{
	return hBitmap.windowBuffer != NULL;
}
BOOL Paint::isSetHBitmapRes()
{
	return flag.isSetHBitmapRes;
}
BOOL Paint::isBackedUpHBitmapWindowBuffer()
{
	return hBitmap.old.windowBuffer != NULL;
}
BOOL Paint::isBackedUpHBitmapRes()
{
	return flag.isBackedUpHBitmapRes;
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
*		- hBitmap management
*
*********************************/
void Paint::hBitmap_windowBuffer_init()
{
	hBitmap.windowBuffer = NULL;
}
void Paint::hBitmap_windowBuffer_set()
{
	if (isSetHBitmapWindowBuffer())
		hBitmap_windowBuffer_release();

	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(hwnd, &windowRect);
	hBitmap.windowBuffer = bmp.create_hDC_compatible(hDC.window, windowRect);
}
void Paint::hBitmap_windowBuffer_release()
{
	if (!isSetHBitmapWindowBuffer()) return;
	//hBitmap을 삭제함
	bmp.delete_hDC_compatible(hBitmap.windowBuffer);
	hBitmap_windowBuffer_init();
}
void Paint::hBitmap_res_init()
{
	for (int i = 0; i < res_count; i++)
		hBitmap.res[i] = NULL;

	//아래 플래그 변수는 삭제할 수 있을 것 같다 
	//(hBitmap.resource.size() != 0)
	flag.isSetHBitmapRes = FALSE;
}
void Paint::hBitmap_res_set()
{
	for (int i = 0; i < res_count; i++)
		hBitmap.res[i] = bmp(i);

	flag.isSetHBitmapRes = TRUE;
}

void Paint::hBitmap_old_windowBuffer_init()
{
	hBitmap.old.windowBuffer = NULL;
}
void Paint::hBitmap_old_windowBuffer_backup()
{
	if (!hDC.mem.windowBuffer.isSet()) return;
	if (isBackedUpHBitmapWindowBuffer())
		hBitmap_old_res_rollback();

	hBitmap.old.windowBuffer
		= (HBITMAP)SelectObject(
			hDC.mem.windowBuffer,
			hBitmap.windowBuffer
		);
}
void Paint::hBitmap_old_windowBuffer_rollback()
{
	if (!isBackedUpHBitmapWindowBuffer()) return;
	SelectObject(
		hDC.mem.windowBuffer, 
		hBitmap.old.windowBuffer
	);
	hBitmap_old_windowBuffer_init();
}
void Paint::hBitmap_old_res_init()
{
	for (int i = 0; i < res_count; i++)
		hBitmap.old.res[i] = NULL;
}
void Paint::hBitmap_old_res_backup()
{
	if (!hDC.mem.res.isSet()) return;
	if (!isBackedUpHBitmapRes())
		hBitmap_old_res_rollback();

	for (int i = 0; i < res_count; i++)
		hBitmap.old.res[i]
		= (HBITMAP)SelectObject(
			hDC.mem.res[i], 
			hBitmap.res[i]
		);

	flag.isBackedUpHBitmapRes = TRUE;
}
void Paint::hBitmap_old_res_rollback()
{
	if (!isBackedUpHBitmapRes()) return;

	for (int i = 0; i < res_count; i++)
		SelectObject(
			hDC.mem.res[i], 
			hBitmap.old.res[i]
		);

	hBitmap_old_res_init();

	flag.isBackedUpHBitmapRes = FALSE;
}



/********************************
*
*		private functions
*		- double buffering manage
*
*********************************/
void Paint::doubleBuffering_init()
{	
	//doublebuffering이 처음 시작되었을때만 아래 작업 수행
	//hDC.memory.windowBuffer와 hDC.mem.res를 생성하는 것과
	//hDC.mem.res에 hBitmap.res를 선택하는 것은 한번만 수행해도 됨
	if (isInitDoubleBuffering()) return;

	//hDC.mem.window와 hDC.mem.res를 생성하고 
	//hBitmap.res를 hDC.mem.res에 선택시킨다
	hDC.mem.create(hDC.window, hDC.mem.windowBuffer);
	hBitmap_old_res_backup();

	//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
	hBitmap_windowBuffer_set();
	hBitmap_old_windowBuffer_backup();

	flag.isInitDoubleBuffering = TRUE;
}
void Paint::doubleBuffering_start()
{
	if (isDoubleBufferingStart()) return;
	if (!hDC.window.isSet()) return;

	//더블버퍼링 초기작업이 안됐으면 우선 수행하도록 한다
	if (!isInitDoubleBuffering())
		doubleBuffering_init();

	//윈도우 크기가 변경되었으면 변경된 크기에 맞는 hBitmap_windowBuffer를 등록한다
	if (isWindowSizeChanged())
	{
		GetClientRect(hwnd, &windowRect);
		//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
		hBitmap_old_windowBuffer_rollback();
		hBitmap_windowBuffer_release();

		//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
		hBitmap_windowBuffer_set();
		hBitmap_old_windowBuffer_backup();

		PixelCoord p(windowRect.right / 2, windowRect.bottom / 2);
		scale.fix_point_pixel(p);

		flag.isWindowSizeChanged = FALSE;
	}

	flag.isDoubleBufferingStart = TRUE;
}
void Paint::doubleBuffering_stop()
{
	if (!isDoubleBufferingStart()) return;

	//아래는 프로그램이 종료될 때 수행되어야 한다
	//doubleBuffering_halt();

	flag.isDoubleBufferingStart = FALSE;
}
void Paint::doubleBuffering_halt()
{
	//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
	hBitmap_old_windowBuffer_rollback();
	hBitmap_windowBuffer_release();

	//doublebuffering을 끝내고 프로그램을 종료할 때 마지막으로 아래 작업 수행
	//hDC.mem.windowBuffer와 hDC.mem.res를 삭제함
	hBitmap_old_res_rollback();
	hDC.mem.del();
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
			StretchBlt(
				hDC.mem.windowBuffer,
				i * px_size + px_pos.x, j * px_size + px_pos.y, px_size, px_size,
				hDC.mem.res[bmp.idx(background, scale)], 0, 0, tsize, tsize,
				SRCCOPY
			);
}
void Paint::paint_background_ruller_tobuffer()
{
	PhysicalVector p;
	int cm = 40;
	auto winBuff = hDC.mem.windowBuffer;

	p(-cm, 0);
	MoveToEx(winBuff, scale.transform(p).x, scale.transform(p).y, NULL);
	p(cm, 0);
	LineTo(winBuff, scale.transform(p).x, scale.transform(p).y);
	p(0, -cm);
	MoveToEx(winBuff, scale.transform(p).x, scale.transform(p).y, NULL);
	p(0, cm);
	LineTo(winBuff, scale.transform(p).x, scale.transform(p).y);


	for (int i = -30; i < 30; i++)
	{
		p(i, 0.5);
		MoveToEx(winBuff, scale.transform(p).x, scale.transform(p).y, NULL);
		p(i, -0.5);
		LineTo(winBuff, scale.transform(p).x, scale.transform(p).y);
	}

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
				hDC.mem.res[bmp.idx(object, scale, mask)], 0, 0, tsize, tsize,
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

