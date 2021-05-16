#include "PrintManager.h"

using namespace RollingBall;

void PrintManager::initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType)
{
	BallSizeType = m_BallSizeType;
	hInstance = m_hInstance;
	hwnd = m_hwnd;

	set_windowDCmode_BeginPaint();
	m_isSetWindowDC = FALSE;
	m_isSetMemoryDC = FALSE;

	bitmapManager.initialize(hInstance);
	set_hBitmap(m_BallSizeType);
}



void PrintManager::set_hBitmap(int BallSizeType)
{
	hBitmap.resource.background = bitmapManager.get_hBitmap_floor();
	hBitmap.resource.ball = bitmapManager.get_hBitmap_ball(BallSizeType);
	hBitmap.resource.ball_mask = bitmapManager.get_hBitmap_ball_mask(BallSizeType);
}

void PrintManager::set_windowDCmode_BeginPaint()
{
	m_isWindowDCmode_GetDC = FALSE;
}

void PrintManager::set_windowDCmode_GetDC()
{
	m_isWindowDCmode_GetDC = TRUE;
}



void PrintManager::set_hDCwindow()
{
	if (m_isSetWindowDC) return;

	if (m_isWindowDCmode_GetDC)
		hDC.window = GetDC(hwnd);
	else
		hDC.window = BeginPaint(hwnd, &ps);

	m_isSetWindowDC = TRUE;
}

void PrintManager::release_hDCwindow()
{
	if (!m_isSetWindowDC) return;

	if (m_isWindowDCmode_GetDC)
		ReleaseDC(hwnd, hDC.window);
	else
		EndPaint(hwnd, &ps);

	m_isSetWindowDC = FALSE;
}



void PrintManager::set_memoryDC()
{
	if (!m_isSetWindowDC) return;
	if (m_isSetMemoryDC) return;

	//화면 DC와 호환이 되는 memDC를 생성
	hDC.memory.hDCwindowCompatible = CreateCompatibleDC(hDC.window);
	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	hDC.memory.background = CreateCompatibleDC(hDC.memory.hDCwindowCompatible);
	hDC.memory.ball = CreateCompatibleDC(hDC.memory.hDCwindowCompatible);
	hDC.memory.ball_mask = CreateCompatibleDC(hDC.memory.hDCwindowCompatible);

	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(hwnd, &windowRect);
	hBitmap.hDCwindowCompatible
		= CreateCompatibleBitmap(hDC.window, windowRect.right, windowRect.bottom);

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

void PrintManager::release_memoryDC()
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

void PrintManager::set_hDC()
{
	set_hDCwindow();
	set_memoryDC();
}

void PrintManager::release_hDC()
{
	release_hDCwindow();
	release_memoryDC();
}



BOOL RollingBall::PrintManager::isWindowDCmode_GetDC()
{
	return m_isWindowDCmode_GetDC;
}

BOOL RollingBall::PrintManager::isSetWindowDC()
{
	return m_isSetWindowDC;
}

BOOL RollingBall::PrintManager::isSetMemoryDC()
{
	return m_isSetMemoryDC;
}

BOOL RollingBall::PrintManager::isAbleToPrint()
{
	if (!isSetWindowDC()) return FALSE;
	if (!isSetMemoryDC()) return FALSE;
	return TRUE;
}

void RollingBall::PrintManager::flush_buffer()
{
	if (!isAbleToPrint()) return;

	BitBlt(
		hDC.window, 0, 0, windowRect.right, windowRect.bottom,
		hDC.memory.hDCwindowCompatible, 0, 0,
		SRCCOPY
	);
}



void PrintManager::set_BallSizeType(int BallSize)
{
	BallSizeType = BallSizeType;
}

void PrintManager::draw_background_tobuffer(LPRECT region)
{
	if (!isAbleToPrint()) return;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 4; j++)
			BitBlt(hDC.memory.hDCwindowCompatible, i * 256, j * 256, 256, 256, hDC.memory.background, 0, 0, SRCCOPY);
}

void PrintManager::draw_ball_tobuffer(int x, int y)
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

void PrintManager::draw_background_ruller_tobuffer()
{

}



PrintManager::~PrintManager()
{

}
