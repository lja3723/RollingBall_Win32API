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

	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;
	if (!bmp.init(winAPI.hInstance)) return FALSE;

	init_flags();
	init_res_count();
	scale_set(32);

	memset(&winAPI.ps, 0, sizeof(winAPI.ps));
	memset(&winAPI.windowRect, 0, sizeof(winAPI.windowRect));

	hDCwindow_init();
	memDC_windowBuffer_init();
	memDC_res_init();

	hBitmap_windowBuffer_init();
	hBitmap_res_init();

	hBitmap_old_windowBuffer_init();
	hBitmap_old_res_init();

	hBitmap_res_set();

	return TRUE;
}
void Paint::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	if (m_iMsg == WM_SIZE)
	{
		switch (m_wParam)
		{
		case SIZE_RESTORED:
		case SIZE_MAXIMIZED:
		case SIZE_MAXSHOW:
			flag.isWindowSizeChanged = TRUE;
			break;
		}
	}
	if (m_iMsg == WM_KEYDOWN)
	{
		PhysicalVector ppos;
		switch (m_wParam)
		{
		case 'O':
			if (scale.px_rate() > 5)
				scale.px_rate(scale.px_rate() - 1);
			break;
		case 'P':
			scale.px_rate(scale.px_rate() + 1);
			break;
		case 'W':
			ppos = scale.fix_point_physical();
			ppos.y -= 0.1;
			scale.fix_point_physical(ppos);
			break;
		case 'A':
			ppos = scale.fix_point_physical();
			ppos.x += 0.1;
			scale.fix_point_physical(ppos);
			break;
		case 'S':
			ppos = scale.fix_point_physical();
			ppos.y += 0.1;
			scale.fix_point_physical(ppos);
			break;
		case 'D':
			ppos = scale.fix_point_physical();
			ppos.x -= 0.1;
			scale.fix_point_physical(ppos);
			break;
		}
	}
}
void RollingBall::Paint::scale_set(pixel px_rate)
{
	scale.px_rate(px_rate);
	PhysicalVector ppos;
	PixelCoord xpos;
	ppos.x = 2;
	ppos.y = 2;
	scale.fix_point_physical(ppos);
	GetClientRect(winAPI.hwnd, &winAPI.windowRect);
	xpos.x = winAPI.windowRect.right / 2;
	xpos.y = winAPI.windowRect.bottom / 2;
	scale.fix_point_pixel(xpos);
}
void Paint::begin()
{
	hDCwindowMode_set_BeginPaint();
	hDCwindow_set();
	doubleBuffering_start();
}
void Paint::end()
{
	flush_buffer();
	hDCwindow_release();
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



/********************************
*
*		private functions
*		- initialization
*
*********************************/
//처음 init 될 때 한 번만 호출되는 함수
void Paint::init_flags()
{
	hDCwindowMode_set_BeginPaint();

	flag.isSetMemDCres = FALSE;
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
	winAPI.hDC.mem.res.resize(res_count);
	winAPI.hBitmap.res.resize(res_count);
	winAPI.hBitmap.old.res.resize(res_count);
}



/********************************
*
*		private functions
*		- BOOL returns
*
*********************************/
BOOL Paint::isInit()
{
	return winAPI.hInstance != NULL;
}

BOOL Paint::isHDCwindowMode_GetDC()
{
	return flag.isHDCwindowMode_GetDC;
}
BOOL Paint::isHDCwindowMode_BeginPaint()
{
	return !isHDCwindowMode_GetDC();
}

BOOL Paint::isSetHDCwindow()
{
	return winAPI.hDC.window != NULL;
}
BOOL Paint::isSetMemDCwindowBuffer()
{
	return winAPI.hDC.mem.windowBuffer != NULL;
}
BOOL Paint::isSetMemDCres()
{
	return flag.isSetMemDCres;
}
BOOL Paint::isSetHBitmapWindowBuffer()
{
	return winAPI.hBitmap.windowBuffer != NULL;
}
BOOL Paint::isSetHBitmapRes()
{
	return flag.isSetHBitmapRes;
}
BOOL Paint::isBackedUpHBitmapWindowBuffer()
{
	return winAPI.hBitmap.old.windowBuffer != NULL;
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
*		- hDCwindow Management
*
*********************************/
void Paint::hDCwindowMode_set_BeginPaint()
{
	flag.isHDCwindowMode_GetDC = FALSE;
}
void Paint::hDCwindowMode_set_GetDC()
{
	flag.isHDCwindowMode_GetDC = TRUE;
}

void Paint::hDCwindow_init()
{
	winAPI.hDC.window = NULL;
}
void Paint::hDCwindow_set()
{
	if (isSetHDCwindow())
		hDCwindow_release();

	if (isHDCwindowMode_GetDC())
		winAPI.hDC.window = GetDC(winAPI.hwnd);
	else
		winAPI.hDC.window = BeginPaint(winAPI.hwnd, &winAPI.ps);
}
void Paint::hDCwindow_release()
{
	if (!isSetHDCwindow()) return;

	if (isHDCwindowMode_GetDC())
		ReleaseDC(winAPI.hwnd, winAPI.hDC.window);
	else
		EndPaint(winAPI.hwnd, &winAPI.ps);

	hDCwindow_init();
}




/********************************
*
*		private functions
*		- hDC.mem management
*
*********************************/
void Paint::memDC_windowBuffer_init()
{
	winAPI.hDC.mem.windowBuffer = NULL;
}
void Paint::memDC_windowBuffer_set()
{	
	if (isSetMemDCwindowBuffer())
		memDC_windowBuffer_release();
	
	//화면 DC와 호환이 되는 memDC를 생성
	winAPI.hDC.mem.windowBuffer = CreateCompatibleDC(winAPI.hDC.window);
}
void Paint::memDC_windowBuffer_release()
{
	if (!isSetMemDCwindowBuffer()) return;

	DeleteDC(winAPI.hDC.mem.windowBuffer);
	memDC_windowBuffer_init();
}

void Paint::memDC_res_init()
{
	for (int i = 0; i < res_count; i++)
		winAPI.hDC.mem.res[i] = NULL;
}
void Paint::memDC_res_set()
{
	if (isSetMemDCres())
		memDC_res_release();

	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	for (int i = 0; i < res_count; i++)
		winAPI.hDC.mem.res[i] = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);

	flag.isSetMemDCres = TRUE;
}
void Paint::memDC_res_release()
{
	if (!isSetMemDCres()) return;

	for (int i = 0; i < res_count; i++)
		DeleteDC(winAPI.hDC.mem.res[i]);

	memDC_res_init();

	flag.isSetMemDCres = FALSE;
}

void Paint::memDC_create()
{
	memDC_windowBuffer_set();
	memDC_res_set();
}
void Paint::memDC_delete()
{
	memDC_windowBuffer_release();
	memDC_res_release();
}



/********************************
*
*		private functions
*		- hBitmap management
*
*********************************/
void Paint::hBitmap_windowBuffer_init()
{
	winAPI.hBitmap.windowBuffer = NULL;
}
void Paint::hBitmap_windowBuffer_set()
{
	if (isSetHBitmapWindowBuffer())
		hBitmap_windowBuffer_release();

	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(winAPI.hwnd, &winAPI.windowRect);
	winAPI.hBitmap.windowBuffer = bmp.create_hDC_compatible(winAPI.hDC.window, winAPI.windowRect);
}
void Paint::hBitmap_windowBuffer_release()
{
	if (!isSetHBitmapWindowBuffer()) return;
	//hBitmap을 삭제함
	bmp.delete_hDC_compatible(winAPI.hBitmap.windowBuffer);
	hBitmap_windowBuffer_init();
}
void Paint::hBitmap_res_init()
{
	for (int i = 0; i < res_count; i++)
		winAPI.hBitmap.res[i] = NULL;

	//아래 플래그 변수는 삭제할 수 있을 것 같다 
	//(winAPI.hBitmap.resource.size() != 0)
	flag.isSetHBitmapRes = FALSE;
}
void Paint::hBitmap_res_set()
{
	for (int i = 0; i < res_count; i++)
		winAPI.hBitmap.res[i] = bmp(i);

	flag.isSetHBitmapRes = TRUE;
}

void Paint::hBitmap_old_windowBuffer_init()
{
	winAPI.hBitmap.old.windowBuffer = NULL;
}
void Paint::hBitmap_old_windowBuffer_backup()
{
	if (!isSetMemDCwindowBuffer()) return;
	if (isBackedUpHBitmapWindowBuffer())
		hBitmap_old_res_rollback();

	winAPI.hBitmap.old.windowBuffer
		= (HBITMAP)SelectObject(
			winAPI.hDC.mem.windowBuffer,
			winAPI.hBitmap.windowBuffer
		);
}
void Paint::hBitmap_old_windowBuffer_rollback()
{
	if (!isBackedUpHBitmapWindowBuffer()) return;
	SelectObject(
		winAPI.hDC.mem.windowBuffer, 
		winAPI.hBitmap.old.windowBuffer
	);
	hBitmap_old_windowBuffer_init();
}
void Paint::hBitmap_old_res_init()
{
	for (int i = 0; i < res_count; i++)
		winAPI.hBitmap.old.res[i] = NULL;
}
void Paint::hBitmap_old_res_backup()
{
	if (!isSetMemDCres()) return;
	if (!isBackedUpHBitmapRes())
		hBitmap_old_res_rollback();

	for (int i = 0; i < res_count; i++)
		winAPI.hBitmap.old.res[i]
		= (HBITMAP)SelectObject(
			winAPI.hDC.mem.res[i], 
			winAPI.hBitmap.res[i]
		);

	flag.isBackedUpHBitmapRes = TRUE;
}
void Paint::hBitmap_old_res_rollback()
{
	if (!isBackedUpHBitmapRes()) return;

	for (int i = 0; i < res_count; i++)
		SelectObject(
			winAPI.hDC.mem.res[i], 
			winAPI.hBitmap.old.res[i]
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
	memDC_create();
	hBitmap_old_res_backup();

	//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
	hBitmap_windowBuffer_set();
	hBitmap_old_windowBuffer_backup();

	flag.isInitDoubleBuffering = TRUE;
}
void Paint::doubleBuffering_start()
{
	if (isDoubleBufferingStart()) return;
	if (!isSetHDCwindow()) return;

	//더블버퍼링 초기작업이 안됐으면 우선 수행하도록 한다
	if (!isInitDoubleBuffering())
		doubleBuffering_init();

	//윈도우 크기가 변경되었으면 변경된 크기에 맞는 hBitmap_windowBuffer를 등록한다
	if (isWindowSizeChanged())
	{
		//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
		hBitmap_old_windowBuffer_rollback();
		hBitmap_windowBuffer_release();

		//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
		hBitmap_windowBuffer_set();
		hBitmap_old_windowBuffer_backup();

		PixelCoord p;
		p.x = winAPI.windowRect.right / 2;
		p.y = winAPI.windowRect.bottom / 2;
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
	memDC_delete();
}




/********************************
*
*		private functions
*		- variable management
*
*********************************/




/********************************
*
*		private functions
*		- paint management
*
*********************************/
void Paint::paint_background_tobuffer(Object& background)
{
	if (!isReadyToPaint()) return;

	GetClientRect(winAPI.hwnd, &winAPI.windowRect);

	cm_val physical_size = background.physical.size;
	pixel px_size = scale.px(physical_size);
	pixel tsize = background.texture_size(scale);
	PixelCoord pos = scale.transform(background.physical.pos);
	pos.x %= px_size;
	pos.y %= px_size;

	SetStretchBltMode(winAPI.hDC.mem.windowBuffer, COLORONCOLOR);
	for (int i = -1; i * px_size + pos.x < winAPI.windowRect.right; i++)
		for (int j = -1; j * px_size + pos.y < winAPI.windowRect.bottom; j++)
			StretchBlt(
				winAPI.hDC.mem.windowBuffer, 
				i * px_size + pos.x, j * px_size + pos.y, px_size, px_size,
				winAPI.hDC.mem.res[bmp.idx(background, scale)], 0, 0, tsize, tsize,
				SRCCOPY
			);
}
void Paint::paint_background_ruller_tobuffer()
{
	PhysicalVector p;
	int cm = 40;
	p.x = -cm, p.y = 0;
	MoveToEx(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y, NULL);
	p.x = cm;
	LineTo(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y);
	p.x = 0, p.y = -cm;
	MoveToEx(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y, NULL);
	p.y = cm;
	LineTo(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y);


	for (int i = -30; i < 30; i++)
	{
		p.x = i, p.y = 0.5;
		MoveToEx(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y, NULL);
		p.y = -0.5;
		LineTo(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y);
	}

	for (int i = -30; i < 30; i++)
	{
		p.y = i, p.x = 0.5;
		MoveToEx(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y, NULL);
		p.x = -0.5;
		LineTo(winAPI.hDC.mem.windowBuffer, scale.transform(p).x, scale.transform(p).y);
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

			cm_val physical_size = object.physical.size;
			pixel px_size = scale.px(physical_size);
			pixel tsize = object.texture_size(scale);
			PixelCoord pos = scale.transform(object.physical.pos);
			int res_idx = bmp.idx(object, scale, mask);
			SetStretchBltMode(winAPI.hDC.mem.windowBuffer, COLORONCOLOR);
			StretchBlt(
				winAPI.hDC.mem.windowBuffer,
				pos.x - px_size / 2, pos.y - px_size / 2, px_size, px_size,
				winAPI.hDC.mem.res[res_idx], 0, 0, tsize, tsize,
				paintmode
			);
		}
	}
}

void Paint::flush_buffer()
{
	if (!isReadyToPaint()) return;

	BitBlt(
		winAPI.hDC.window, 
		0, 0, winAPI.windowRect.right, winAPI.windowRect.bottom,
		winAPI.hDC.mem.windowBuffer, 0, 0,
		SRCCOPY
	);
}

