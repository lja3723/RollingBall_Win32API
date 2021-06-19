#include "paint.h"
#include "debugger.h"
using namespace RollingBall;




//////////////////////////////////
//oldcode
//////////////////////////////////
//static variables
//int Paint::_oldcode_res_count = 0;


//처음 init 될 때 한 번만 호출되는 함수
//void Paint::_oldcode_init_res_count()
//{
//	if (!_oldcode_bmp.isInit()) return;
//	_oldcode_res_count = _oldcode_bmp.file_count();
//	init_res_vectors();
//}


BOOL Paint::_oldcode_isSetHBitmapWindowBuffer()
{
	return _oldcode_hBitmap.windowBuffer != NULL;
}
BOOL Paint::_oldcode_isSetHBitmapRes()
{
	return _oldcode_flag_isSetHBitmapRes;
}
BOOL Paint::_oldcode_isBackedUpHBitmapWindowBuffer()
{
	return _oldcode_hBitmap.old.windowBuffer != NULL;
}
BOOL Paint::_oldcode_isBackedUpHBitmapRes()
{
	return _oldcode_flag_isBackedUpHBitmapRes;
}


void Paint::_oldcode_hBitmap_windowBuffer_init()
{
	_oldcode_hBitmap.windowBuffer = NULL;
}
void Paint::_oldcode_hBitmap_windowBuffer_set()
{
	if (_oldcode_isSetHBitmapWindowBuffer())
		_oldcode_hBitmap_windowBuffer_release();

	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(hwnd, &windowRect);
	_oldcode_hBitmap.windowBuffer = _oldcode_bmp.create_hDC_compatible(hDC.window, windowRect);
}
void Paint::_oldcode_hBitmap_windowBuffer_release()
{
	if (!_oldcode_isSetHBitmapWindowBuffer()) return;
	//hBitmap을 삭제함
	_oldcode_bmp.delete_hDC_compatible(_oldcode_hBitmap.windowBuffer);
	_oldcode_hBitmap_windowBuffer_init();
}
void Paint::_oldcode_hBitmap_res_init()
{
	for (int i = 0; i < hBitmap.res_count(); i++)
		_oldcode_hBitmap.res[i] = NULL;

	//아래 플래그 변수는 삭제할 수 있을 것 같다 
	//(hBitmap.resource.size() != 0)
	_oldcode_flag_isSetHBitmapRes = FALSE;
}
void Paint::_oldcode_hBitmap_res_set()
{
	for (int i = 0; i < hBitmap.res_count(); i++)
		_oldcode_hBitmap.res[i] = _oldcode_bmp(i);

	_oldcode_flag_isSetHBitmapRes = TRUE;
}

void Paint::_oldcode_hBitmap_old_windowBuffer_init()
{
	_oldcode_hBitmap.old.windowBuffer = NULL;
}
void Paint::_oldcode_hBitmap_old_windowBuffer_backup()
{
	if (!hDC.mem.windowBuffer.isSet()) return;
	if (_oldcode_isBackedUpHBitmapWindowBuffer())
		_oldcode_hBitmap_old_res_rollback();

	_oldcode_hBitmap.old.windowBuffer
		= (HBITMAP)SelectObject(
			hDC.mem.windowBuffer,
			_oldcode_hBitmap.windowBuffer
		);
}
void Paint::_oldcode_hBitmap_old_windowBuffer_rollback()
{
	if (!_oldcode_isBackedUpHBitmapWindowBuffer()) return;
	SelectObject(
		hDC.mem.windowBuffer,
		_oldcode_hBitmap.old.windowBuffer
	);
	_oldcode_hBitmap_old_windowBuffer_init();
}
void Paint::_oldcode_hBitmap_old_res_init()
{
	for (int i = 0; i < hBitmap.res_count(); i++)
		_oldcode_hBitmap.old.res[i] = NULL;
}
void Paint::_oldcode_hBitmap_old_res_backup()
{
	if (!hDC.mem.res.isSet()) return;
	if (!_oldcode_isBackedUpHBitmapRes())
		_oldcode_hBitmap_old_res_rollback();

	for (int i = 0; i < hBitmap.res_count(); i++)
		_oldcode_hBitmap.old.res[i]
		= (HBITMAP)SelectObject(
			hDC.mem.res[i],
			_oldcode_hBitmap.res[i]
		);

	_oldcode_flag_isBackedUpHBitmapRes = TRUE;
}
void Paint::_oldcode_hBitmap_old_res_rollback()
{
	if (!_oldcode_isBackedUpHBitmapRes()) return;

	for (int i = 0; i < hBitmap.res_count(); i++)
		SelectObject(
			hDC.mem.res[i],
			_oldcode_hBitmap.old.res[i]
		);

	_oldcode_hBitmap_old_res_init();

	_oldcode_flag_isBackedUpHBitmapRes = FALSE;
}
//////////////////////////////////
//~oldcode
//////////////////////////////////



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

	//oldcode
	if (!_oldcode_bmp.init(hInstance)) return FALSE;
	//_oldcode_init_res_count();
	//~oldcode//newcode
	if (!hBitmap.init(hInstance)) return FALSE;
	init_res_vectors(hBitmap.res_count());
	//~newcode

	set_px_rate(32);
	GetClientRect(hwnd, &windowRect);
	set_fix_point(PixelCoord(windowRect.right / 2, windowRect.bottom / 2));
	set_fix_point(PhysicalVector(0, 0));

	//oldcode
	//아래코드는 생성자로 대체되었음
	_oldcode_hBitmap_windowBuffer_init();
	_oldcode_hBitmap_old_windowBuffer_init();

	_oldcode_hBitmap_res_init();
	_oldcode_hBitmap_old_res_init();

	//아래 코드만 새코드로 코딩하면됨
	_oldcode_hBitmap_res_set();
	//~oldcode//newcode
	hBitmap.res.set();
	//~newcode

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
//처음 init 될 때 한 번만 호출되는 함수
void Paint::init_res_vectors(int res_count)
{
	//oldcode
	_oldcode_hBitmap.res.resize(res_count);
	_oldcode_hBitmap.old.res.resize(res_count);
	//~oldcode//newcode
	hBitmap.resize_vectors(res_count);
	//~newcode
	hDC.mem.res.resize(res_count);
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
	//doublebuffering이 처음 시작되었을때만 아래 작업 수행
	//hDC.memory.windowBuffer와 hDC.mem.res를 생성하는 것과
	//hDC.mem.res에 hBitmap.res를 선택하는 것은 한번만 수행해도 됨
	if (isInitDoubleBuffering()) return;

	//hDC.mem.window와 hDC.mem.res를 생성하고 
	//hBitmap.res를 hDC.mem.res에 선택시킨다
	hDC.mem.create(hDC.window, hDC.mem.windowBuffer);
	//oldcode
	_oldcode_hBitmap_old_res_backup();

	//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
	_oldcode_hBitmap_windowBuffer_set();
	_oldcode_hBitmap_old_windowBuffer_backup();
	//~oldcode//newcode
	hBitmap.res.old.backup(hDC);

	//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
	hBitmap.windowBuffer.set(hwnd, hDC.window);
	hBitmap.windowBuffer.old.backup(hDC);
	//~newcode

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
		//oldcode
		//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
		//_oldcode_hBitmap_old_windowBuffer_rollback();
		//_oldcode_hBitmap_windowBuffer_release();

		//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
		//_oldcode_hBitmap_windowBuffer_set();
		//_oldcode_hBitmap_old_windowBuffer_backup();
		//~oldcode//newcode
		//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
		hBitmap.windowBuffer.old.rollback(hDC);
		hBitmap.windowBuffer.release();
		//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
		hBitmap.windowBuffer.set(hwnd, hDC.window);
		hBitmap.windowBuffer.old.backup(hDC);
		//~newcode


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
	//oldcode
	//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
	//_oldcode_hBitmap_old_windowBuffer_rollback();
	//_oldcode_hBitmap_windowBuffer_release();

	//doublebuffering을 끝내고 프로그램을 종료할 때 마지막으로 아래 작업 수행
	//hDC.mem.windowBuffer와 hDC.mem.res를 삭제함
	//_oldcode_hBitmap_old_res_rollback();
	//~oldcode//newcode
	//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
	hBitmap.windowBuffer.old.rollback(hDC);
	hBitmap.windowBuffer.release();

	//doublebuffering을 끝내고 프로그램을 종료할 때 마지막으로 아래 작업 수행
	//hDC.mem.windowBuffer와 hDC.mem.res를 삭제함
	hBitmap.res.old.rollback(hDC);
	//~newcode

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
		{
			//oldcode
			//StretchBlt(
			//	hDC.mem.windowBuffer,
			//	i * px_size + px_pos.x, j * px_size + px_pos.y, px_size, px_size,
			//	hDC.mem.res[_oldcode_bmp.idx(background, scale)], 0, 0, tsize, tsize,
			//	SRCCOPY
			//);
			//~oldcode//newcode
			StretchBlt(
				hDC.mem.windowBuffer,
				i * px_size + px_pos.x, j * px_size + px_pos.y, px_size, px_size,
				hDC.mem.res[hBitmap.bmpidx(background, scale)], 0, 0, tsize, tsize,
				SRCCOPY
			);
			//~newcode
		}
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
			//oldcode
			//StretchBlt(
			//	hDC.mem.windowBuffer,
			//	px_pos.x - px_size / 2, px_pos.y - px_size / 2, px_size, px_size,
			//	hDC.mem.res[_oldcode_bmp.idx(object, scale, mask)], 0, 0, tsize, tsize,
			//	paintmode
			//);
			//~oldcode//newcode
			StretchBlt(
				hDC.mem.windowBuffer,
				px_pos.x - px_size / 2, px_pos.y - px_size / 2, px_size, px_size,
				hDC.mem.res[hBitmap.bmpidx(object, scale, mask)], 0, 0, tsize, tsize,
				paintmode
			);
			//~newcode
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

