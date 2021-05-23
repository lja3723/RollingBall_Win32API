﻿#include "PaintManager.h"
#include "Debugger.h"
using namespace RollingBall;



/********************************
*
*		static variables
*		initialization
*
*********************************/



/********************************
* 
*		public functions
*
*********************************/
PaintManager::~PaintManager()
{
	doubleBuffering_halt();
}
BOOL PaintManager::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	winAPI.hInstance = m_hInstance;
	winAPI.hwnd = m_hwnd;
	if (!bmp.init(winAPI.hInstance, winAPI.hwnd)) return FALSE;

	init_flags();
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

	flag.isInit = TRUE;
	return TRUE;
}
void PaintManager::beginPaint()
{
	hDCwindowMode_set_BeginPaint();
	hDCwindow_set();
	doubleBuffering_start();
}
void PaintManager::endPaint()
{
	flush_buffer();
	hDCwindow_release();
	doubleBuffering_stop();
}
void PaintManager::paint_background()
{
	paint_background_tobuffer();
	paint_background_ruller_tobuffer();
}
void PaintManager::paint_ball(int posX, int posY, int ballsize)
{
	paint_ball_tobuffer(posX, posY, ballsize);
}



/********************************
*
*		private functions
*		- initialization
*
*********************************/
//처음 init 될 때 한 번만 호출되는 함수
void PaintManager::init_flags()
{
	hDCwindowMode_set_BeginPaint();

	flag.isSetMemDCres = FALSE;
	flag.isSetHBitmapRes = FALSE;
	flag.isBackedUpHBitmapRes = FALSE;

	flag.isDoubleBufferingStart = FALSE;
	flag.isInit = FALSE;
	flag.isInitDoubleBuffering = FALSE;
}



/********************************
*
*		private functions
*		- BOOL returns
*
*********************************/
BOOL PaintManager::isInit()
{
	return flag.isInit;
}

BOOL PaintManager::isHDCwindowMode_GetDC()
{
	return flag.isHDCwindowMode_GetDC;
}
BOOL PaintManager::isHDCwindowMode_BeginPaint()
{
	return !isHDCwindowMode_GetDC();
}

BOOL PaintManager::isSetHDCwindow()
{
	return winAPI.hDC.window != NULL;
}
BOOL PaintManager::isSetMemDCwindowBuffer()
{
	return winAPI.hDC.mem.windowBuffer != NULL;
}
BOOL PaintManager::isSetMemDCres()
{
	return flag.isSetMemDCres;
}
BOOL PaintManager::isSetHBitmapWindowBuffer()
{
	return winAPI.hBitmap.windowBuffer != NULL;
}
BOOL PaintManager::isSetHBitmapRes()
{
	return flag.isSetHBitmapRes;
}
BOOL PaintManager::isBackedUpHBitmapWindowBuffer()
{
	return winAPI.hBitmap.old.windowBuffer != NULL;
}
BOOL PaintManager::isBackedUpHBitmapRes()
{
	return flag.isBackedUpHBitmapRes;
}

BOOL PaintManager::isDoubleBufferingStart()
{
	return flag.isDoubleBufferingStart;
}
BOOL PaintManager::isReadyToPaint()
{
	return isDoubleBufferingStart();
}
BOOL PaintManager::isInitDoubleBuffering()
{
	return flag.isInitDoubleBuffering;
}




/********************************
*
*		private functions
*		- hDCwindow Management
*
*********************************/
void PaintManager::hDCwindowMode_set_BeginPaint()
{
	flag.isHDCwindowMode_GetDC = FALSE;
}
void PaintManager::hDCwindowMode_set_GetDC()
{
	flag.isHDCwindowMode_GetDC = TRUE;
}

void PaintManager::hDCwindow_init()
{
	winAPI.hDC.window = NULL;
}
void PaintManager::hDCwindow_set()
{
	if (isSetHDCwindow())
		hDCwindow_release();

	if (isHDCwindowMode_GetDC())
		winAPI.hDC.window = GetDC(winAPI.hwnd);
	else
		winAPI.hDC.window = BeginPaint(winAPI.hwnd, &winAPI.ps);
}
void PaintManager::hDCwindow_release()
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
void PaintManager::memDC_windowBuffer_init()
{
	winAPI.hDC.mem.windowBuffer = NULL;
}
void PaintManager::memDC_windowBuffer_set()
{	
	if (isSetMemDCwindowBuffer())
		memDC_windowBuffer_release();
	
	//화면 DC와 호환이 되는 memDC를 생성
	winAPI.hDC.mem.windowBuffer = CreateCompatibleDC(winAPI.hDC.window);
}
void PaintManager::memDC_windowBuffer_release()
{
	if (!isSetMemDCwindowBuffer()) return;

	DeleteDC(winAPI.hDC.mem.windowBuffer);
	memDC_windowBuffer_init();
}

void PaintManager::memDC_res_init()
{
	int files = bmp.get_bitmap_file_count();
	winAPI.hDC.mem.resource.resize(files);
	for (int i = 0; i < winAPI.hDC.mem.resource.size(); i++)
		winAPI.hDC.mem.resource[i] = NULL;
}
void PaintManager::memDC_res_set()
{
	if (isSetMemDCres())
		memDC_res_release();

	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	for (int i = 0; i < winAPI.hDC.mem.resource.size(); i++)
		winAPI.hDC.mem.resource[i] = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);

	flag.isSetMemDCres = TRUE;
}
void PaintManager::memDC_res_release()
{
	if (!isSetMemDCres()) return;

	for (int i = 0; i < winAPI.hDC.mem.resource.size(); i++)
		DeleteDC(winAPI.hDC.mem.resource[i]);

	memDC_res_init();

	flag.isSetMemDCres = FALSE;
}

void RollingBall::PaintManager::memDC_create()
{
	memDC_windowBuffer_set();
	memDC_res_set();
}
void RollingBall::PaintManager::memDC_delete()
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
void PaintManager::hBitmap_windowBuffer_init()
{
	winAPI.hBitmap.windowBuffer = NULL;
}
void PaintManager::hBitmap_windowBuffer_set()
{
	if (isSetHBitmapWindowBuffer())
		hBitmap_windowBuffer_release();

	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(winAPI.hwnd, &winAPI.windowRect);
	winAPI.hBitmap.windowBuffer = bmp.create_hDC_compatible(winAPI.hDC.window, winAPI.windowRect);
}
void PaintManager::hBitmap_windowBuffer_release()
{
	if (!isSetHBitmapWindowBuffer()) return;
	//hBitmap을 삭제함
	bmp.delete_hDC_compatible(winAPI.hBitmap.windowBuffer);
	hBitmap_windowBuffer_init();
}
void PaintManager::hBitmap_res_init()
{
	int files = bmp.get_bitmap_file_count();
	winAPI.hBitmap.resource.resize(files);

	for (int i = 0; i < files; i++)
		winAPI.hBitmap.resource[i] = NULL;

	//아래 플래그 변수는 삭제할 수 있을 것 같다 
	//(winAPI.hBitmap.resource.size() != 0)
	flag.isSetHBitmapRes = FALSE;
}
void PaintManager::hBitmap_res_set()
{
	for (int i = 0; i < winAPI.hBitmap.resource.size(); i++)
		winAPI.hBitmap.resource[i] = bmp[i];

	flag.isSetHBitmapRes = TRUE;
}

void PaintManager::hBitmap_old_windowBuffer_init()
{
	winAPI.hBitmap.old.windowBuffer = NULL;
}
void PaintManager::hBitmap_old_windowBuffer_backup()
{
	if (!isSetMemDCwindowBuffer()) return;
	if (isBackedUpHBitmapWindowBuffer())
		hBitmap_old_res_rollback();

	winAPI.hBitmap.old.windowBuffer
		= (HBITMAP)SelectObject(winAPI.hDC.mem.windowBuffer, winAPI.hBitmap.windowBuffer);
}
void PaintManager::hBitmap_old_windowBuffer_rollback()
{
	if (!isBackedUpHBitmapWindowBuffer()) return;
	SelectObject(winAPI.hDC.mem.windowBuffer, winAPI.hBitmap.old.windowBuffer);
	hBitmap_old_windowBuffer_init();
}
void PaintManager::hBitmap_old_res_init()
{
	int files = bmp.get_bitmap_file_count();
	winAPI.hBitmap.old.resource.resize(files);

	for (int i = 0; i < files; i++)
		winAPI.hBitmap.old.resource[i] = NULL;
}
void PaintManager::hBitmap_old_res_backup()
{
	if (!isSetMemDCres()) return;
	if (!isBackedUpHBitmapRes())
		hBitmap_old_res_rollback();

	for (int i = 0; i < winAPI.hBitmap.old.resource.size(); i++)
		winAPI.hBitmap.old.resource[i]
		= (HBITMAP)SelectObject(winAPI.hDC.mem.resource[i], winAPI.hBitmap.resource[i]);

	flag.isBackedUpHBitmapRes = TRUE;
}
void PaintManager::hBitmap_old_res_rollback()
{
	if (!isBackedUpHBitmapRes()) return;

	for (int i = 0; i < winAPI.hBitmap.old.resource.size(); i++)
		SelectObject(winAPI.hDC.mem.resource[i], winAPI.hBitmap.old.resource[i]);

	hBitmap_old_res_init();

	flag.isBackedUpHBitmapRes = FALSE;
}



/********************************
*
*		private functions
*		- double buffering manage
*
*********************************/
void PaintManager::doubleBuffering_init()
{	
	//doublebuffering이 처음 시작되었을때만 아래 작업 수행
	//hDC.memory.windowBuffer와 hDC.mem.res를 생성하는 것과
	//hDC.mem.res에 hBitmap.res를 선택하는 것은 한번만 수행해도 됨
	if (isInitDoubleBuffering()) return;

	//hDC.mem.window와 hDC.mem.res를 생성하고 
	//hBitmap.res를 hDC.mem.res에 선택시킨다
	memDC_create();
	hBitmap_old_res_backup();

	flag.isInitDoubleBuffering = TRUE;
}
void PaintManager::doubleBuffering_start()
{
	if (isDoubleBufferingStart()) return;
	if (!isSetHDCwindow()) return;

	//더블버퍼링 초기작업이 안됐으면 우선 수행하도록 한다
	if (!isInitDoubleBuffering())
		doubleBuffering_init();

	//hBitmap.windowBuffer를 생성한 뒤 그것을 hDC.mem.windowBuffer에 선택시킨다
	hBitmap_windowBuffer_set();
	hBitmap_old_windowBuffer_backup();

	flag.isDoubleBufferingStart = TRUE;
}
void PaintManager::doubleBuffering_stop()
{
	if (!isDoubleBufferingStart()) return;

	//hBitmap.windowBuffer를 hDC.mem.windowBuffer에서 롤백하고 release한다
	hBitmap_old_windowBuffer_rollback();
	hBitmap_windowBuffer_release();

	//아래는 프로그램이 종료될 때 수행되어야 한다
	//doubleBuffering_halt();

	flag.isDoubleBufferingStart = FALSE;
}
void PaintManager::doubleBuffering_halt()
{
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
void PaintManager::paint_background_tobuffer()
{
	if (!isReadyToPaint()) return;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 4; j++)
			BitBlt(
				winAPI.hDC.mem.windowBuffer, i * 256, j * 256, 256, 256,
				winAPI.hDC.mem.resource[bmp.index(_T("floor"), _T("wood1"), 256, FALSE)], 0, 0,
				SRCCOPY
			);
}
void PaintManager::paint_background_ruller_tobuffer()
{

}
void PaintManager::paint_ball_tobuffer(int x, int y, int ballsize)
{
	if (!isReadyToPaint()) return;

	for (int i = 0; i < 2; i++)
	{
		BOOL mask;
		DWORD paintmode;
		if (i == 0)		
			mask = TRUE, paintmode = SRCAND;
		else if (i == 1)
			mask = FALSE, paintmode = SRCPAINT; 
		
		int idx = bmp.index(_T("ball"), _T("iron1"), ballsize, mask);

		BitBlt(
			winAPI.hDC.mem.windowBuffer,
			x - ballsize / 2, y - ballsize / 2, ballsize, ballsize,
			winAPI.hDC.mem.resource[idx], 0, 0,
			paintmode
		);
	}
}

void PaintManager::flush_buffer()
{
	if (!isReadyToPaint()) return;

	BitBlt(
		winAPI.hDC.window, 
		0, 0, winAPI.windowRect.right, winAPI.windowRect.bottom,
		winAPI.hDC.mem.windowBuffer, 0, 0,
		SRCCOPY
	);
}


