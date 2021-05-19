#include "PaintManager.h"
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

}
void PaintManager::initialize(HINSTANCE hInstance, HWND hwnd, int m_BallSizeType)
{
	winAPI.hInstance = hInstance;
	winAPI.hwnd = hwnd;
	memset(&winAPI.ps, 0, sizeof(winAPI.ps));
	memset(&winAPI.windowRect, 0, sizeof(winAPI.windowRect));

	set_windowDCmode_BeginPaint();
	flag.isSetWindowDC = FALSE;
	flag.isSetMemoryDC = FALSE;

	bitmapManager.initialize(winAPI.hInstance);
	BallSizeType = m_BallSizeType;

	set_hBitmap(m_BallSizeType);
}
void PaintManager::beginPaint()
{
	set_windowDCmode_BeginPaint();
	set_hDCwindow();
	set_memoryDC();
}
void PaintManager::endPaint()
{
	flush_buffer();
	release_hDCwindow();
	release_memoryDC();
}
void PaintManager::paint_background()
{
	paint_background_tobuffer(NULL);
	paint_background_ruller_tobuffer();
}
void PaintManager::paint_ball(int posX, int posY)
{
	paint_ball_tobuffer(posX, posY);
}


/********************************
*
*		private functions
*		- BOOL returns
*
*********************************/
BOOL PaintManager::isWindowDCmode_GetDC()
{
	return flag.isWindowDCmode_GetDC;
}
BOOL PaintManager::isSetWindowDC()
{
	return flag.isSetWindowDC;
}
BOOL PaintManager::isSetMemoryDC()
{
	return flag.isSetMemoryDC;
}
BOOL PaintManager::isAbleToPrint()
{
	if (!isSetWindowDC()) return FALSE;
	if (!isSetMemoryDC()) return FALSE;
	return TRUE;
}


/********************************
*
*		private functions
*		- set windowDCmode
* 
*********************************/
void PaintManager::set_windowDCmode_BeginPaint()
{
	flag.isWindowDCmode_GetDC = FALSE;
}
void PaintManager::set_windowDCmode_GetDC()
{
	flag.isWindowDCmode_GetDC = TRUE;
}


/********************************
*
*		private functions
*		- Manage hDCwindow
*
*********************************/
void PaintManager::set_hDCwindow()
{
	if (isSetWindowDC()) return;

	if (isWindowDCmode_GetDC())
		winAPI.hDC.window = GetDC(winAPI.hwnd);
	else
		winAPI.hDC.window = BeginPaint(winAPI.hwnd, &winAPI.ps);

	flag.isSetWindowDC = TRUE;
}
void PaintManager::release_hDCwindow()
{
	if (!isSetWindowDC()) return;

	if (isWindowDCmode_GetDC())
		ReleaseDC(winAPI.hwnd, winAPI.hDC.window);
	else
		EndPaint(winAPI.hwnd, &winAPI.ps);

	flag.isSetWindowDC = FALSE;
}


/********************************
*
*		private functions
*		- Manage memoryDC
*
*********************************/
void PaintManager::set_memoryDC()
{
	if (!isSetWindowDC()) return;
	if (isSetMemoryDC()) return;

	//화면 DC와 호환이 되는 memDC를 생성
	winAPI.hDC.mem.windowBuffer = CreateCompatibleDC(winAPI.hDC.window);
	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	winAPI.hDC.mem.res.background = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);
	winAPI.hDC.mem.res.ball = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);
	winAPI.hDC.mem.res.ball_mask = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);

	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(winAPI.hwnd, &winAPI.windowRect);
	winAPI.hBitmap.windowBuffer
		= CreateCompatibleBitmap(winAPI.hDC.window, winAPI.windowRect.right, winAPI.windowRect.bottom);

	//생성한 memory DC들에 hBitmap을 선택시킴
	winAPI.hBitmap.old.windowBuffer
		= (HBITMAP)SelectObject(winAPI.hDC.mem.windowBuffer, winAPI.hBitmap.windowBuffer);
	winAPI.hBitmap.old.res.background
		= (HBITMAP)SelectObject(winAPI.hDC.mem.res.background, winAPI.hBitmap.res.background);
	winAPI.hBitmap.old.res.ball
		= (HBITMAP)SelectObject(winAPI.hDC.mem.res.ball, winAPI.hBitmap.res.ball);
	winAPI.hBitmap.old.res.ball_mask
		= (HBITMAP)SelectObject(winAPI.hDC.mem.res.ball_mask, winAPI.hBitmap.res.ball_mask);

	flag.isSetMemoryDC = TRUE;
}
void PaintManager::release_memoryDC()
{
	if (!isSetMemoryDC()) return;
	//holding에 저장된 각 memoryDC의 기본 hBitmap을 선택함
	SelectObject(winAPI.hDC.mem.windowBuffer, winAPI.hBitmap.old.windowBuffer);
	SelectObject(winAPI.hDC.mem.res.background, winAPI.hBitmap.old.res.background);
	SelectObject(winAPI.hDC.mem.res.ball, winAPI.hBitmap.old.res.ball);
	SelectObject(winAPI.hDC.mem.res.ball_mask, winAPI.hBitmap.old.res.ball_mask);

	//memDC들을 삭제함
	DeleteDC(winAPI.hDC.mem.windowBuffer);
	DeleteDC(winAPI.hDC.mem.res.background);
	DeleteDC(winAPI.hDC.mem.res.ball);
	DeleteDC(winAPI.hDC.mem.res.ball_mask);

	//hBitmap을 삭제함
	DeleteObject(winAPI.hBitmap.windowBuffer);

	flag.isSetMemoryDC = FALSE;
}


/********************************
*
*		private functions
*		- set member variable
*
*********************************/
void PaintManager::set_BallSizeType(int BallSize)
{
	BallSizeType = BallSizeType;
}
void PaintManager::set_hBitmap(int BallSizeType)
{
	bitmapManager.set_BallSizeType(BallSizeType);
	winAPI.hBitmap.res.background = bitmapManager.get_hBitmap_floor();
	winAPI.hBitmap.res.ball = bitmapManager.get_hBitmap_ball();
	winAPI.hBitmap.res.ball_mask = bitmapManager.get_hBitmap_ball_mask();
}


/********************************
*
*		private functions
*		- paint to buffer
*
*********************************/
void PaintManager::paint_background_tobuffer(LPRECT region)
{
	if (!isAbleToPrint()) return;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 4; j++)
			BitBlt(
				winAPI.hDC.mem.windowBuffer, i * 256, j * 256, 256, 256, 
				winAPI.hDC.mem.res.background, 0, 0,
				SRCCOPY
			);
}
void PaintManager::paint_ball_tobuffer(int x, int y)
{
	if (!isAbleToPrint()) return;

	BitBlt(
		winAPI.hDC.mem.windowBuffer,
		x - BallSizeType / 2, y - BallSizeType / 2,
		BallSizeType, BallSizeType,
		winAPI.hDC.mem.res.ball_mask, 0, 0, SRCAND
	);

	BitBlt(
		winAPI.hDC.mem.windowBuffer,
		x - BallSizeType / 2, y - BallSizeType / 2,
		BallSizeType, BallSizeType,
		winAPI.hDC.mem.res.ball, 0, 0, SRCPAINT
	);
}
void PaintManager::paint_background_ruller_tobuffer()
{

}


/********************************
*
*		private functions
*		- flush buffer to window
*
*********************************/
void RollingBall::PaintManager::flush_buffer()
{
	if (!isAbleToPrint()) return;

	BitBlt(
		winAPI.hDC.window, 0, 0, winAPI.windowRect.right, winAPI.windowRect.bottom,
		winAPI.hDC.mem.windowBuffer, 0, 0,
		SRCCOPY
	);
}