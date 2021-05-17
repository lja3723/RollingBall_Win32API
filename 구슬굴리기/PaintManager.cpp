#include "PaintManager.h"
using namespace RollingBall;




/********************************
* 
*		public functions
*
*********************************/
void PaintManager::initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType)
{
	BallSizeType = m_BallSizeType;
	hInstance = m_hInstance;
	hwnd = m_hwnd;

	set_windowDCmode_BeginPaint();
	m_isSetWindowDC = FALSE;
	m_isSetMemoryDC = FALSE;

	bitmapManager.initialize(hInstance, hwnd);
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
	return m_isWindowDCmode_GetDC;
}
BOOL PaintManager::isSetWindowDC()
{
	return m_isSetWindowDC;
}
BOOL PaintManager::isSetMemoryDC()
{
	return m_isSetMemoryDC;
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
	m_isWindowDCmode_GetDC = FALSE;
}
void PaintManager::set_windowDCmode_GetDC()
{
	m_isWindowDCmode_GetDC = TRUE;
}


/********************************
*
*		private functions
*		- Manage hDCwindow
*
*********************************/
void PaintManager::set_hDCwindow()
{
	if (m_isSetWindowDC) return;

	if (m_isWindowDCmode_GetDC)
		hDC.window = GetDC(hwnd);
	else
		hDC.window = BeginPaint(hwnd, &ps);

	m_isSetWindowDC = TRUE;
}
void PaintManager::release_hDCwindow()
{
	if (!m_isSetWindowDC) return;

	if (m_isWindowDCmode_GetDC)
		ReleaseDC(hwnd, hDC.window);
	else
		EndPaint(hwnd, &ps);

	m_isSetWindowDC = FALSE;
}


/********************************
*
*		private functions
*		- Manage memoryDC
*
*********************************/
void PaintManager::set_memoryDC()
{
	if (!m_isSetWindowDC) return;
	if (m_isSetMemoryDC) return;

	//화면 DC와 호환이 되는 memDC를 생성
	hDC.memory.hDCwindowCompatible = CreateCompatibleDC(hDC.window);
	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	hDC.memory.background = CreateCompatibleDC(hDC.memory.hDCwindowCompatible);
	hDC.memory.ball = CreateCompatibleDC(hDC.memory.hDCwindowCompatible);
	hDC.memory.ball_mask = CreateCompatibleDC(hDC.memory.hDCwindowCompatible);

	set_hBitmap(BallSizeType);

	//생성한 memory DC들에 hBitmap을 선택시킴
	hBitmap.holdingOld.hDCwindowCompatible
		= (HBITMAP)SelectObject(hDC.memory.hDCwindowCompatible, hBitmap.hDCwindowCompatible);
	hBitmap.holdingOld.background
		= (HBITMAP)SelectObject(hDC.memory.background, hBitmap.resource.background);
	hBitmap.holdingOld.ball
		= (HBITMAP)SelectObject(hDC.memory.ball, hBitmap.resource.ball);
	hBitmap.holdingOld.ball_mask
		= (HBITMAP)SelectObject(hDC.memory.ball_mask, hBitmap.resource.ball_mask);

	m_isSetMemoryDC = TRUE;
}
void PaintManager::release_memoryDC()
{
	if (!m_isSetMemoryDC) return;
	//holding에 저장된 각 memoryDC의 기본 hBitmap을 선택함
	SelectObject(hDC.memory.hDCwindowCompatible, hBitmap.holdingOld.hDCwindowCompatible);
	SelectObject(hDC.memory.background, hBitmap.holdingOld.background);
	SelectObject(hDC.memory.ball, hBitmap.holdingOld.ball);
	SelectObject(hDC.memory.ball_mask, hBitmap.holdingOld.ball_mask);

	//memDC들을 삭제함
	DeleteDC(hDC.memory.hDCwindowCompatible);
	DeleteDC(hDC.memory.background);
	DeleteDC(hDC.memory.ball);
	DeleteDC(hDC.memory.ball_mask);

	m_isSetMemoryDC = FALSE;
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
	hBitmap.resource.background = bitmapManager.get_hBitmap_floor();
	hBitmap.resource.ball = bitmapManager.get_hBitmap_ball();
	hBitmap.resource.ball_mask = bitmapManager.get_hBitmap_ball_mask();
	hBitmap.hDCwindowCompatible = bitmapManager.get_hBitmap_hDCwindowBuffer();
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
			BitBlt(hDC.memory.hDCwindowCompatible, i * 256, j * 256, 256, 256, hDC.memory.background, 0, 0, SRCCOPY);
}
void PaintManager::paint_ball_tobuffer(int x, int y)
{
	if (!isAbleToPrint()) return;

	BitBlt(
		hDC.memory.hDCwindowCompatible,
		x - BallSizeType / 2, y - BallSizeType / 2,
		BallSizeType, BallSizeType,
		hDC.memory.ball_mask, 0, 0, SRCAND
	);

	BitBlt(
		hDC.memory.hDCwindowCompatible,
		x - BallSizeType / 2, y - BallSizeType / 2,
		BallSizeType, BallSizeType,
		hDC.memory.ball, 0, 0, SRCPAINT
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
		hDC.window, 0, 0, windowRect.right, windowRect.bottom,
		hDC.memory.hDCwindowCompatible, 0, 0,
		SRCCOPY
	);
}