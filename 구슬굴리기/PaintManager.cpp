#include "PaintManager.h"
using namespace RollingBall;



/********************************
*
*		static variables
*		initialization
*
*********************************/
int PaintManager::isCreatedMemoryDC = FALSE;


/********************************
* 
*		public functions
*
*********************************/
RollingBall::PaintManager::~PaintManager()
{
	if (isCreatedMemoryDC)
	{
		release_ObjectMemoryDC();
		isCreatedMemoryDC = FALSE;
	}
}
void PaintManager::initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType)
{
	BallSizeType = m_BallSizeType;
	hInstance = m_hInstance;
	hwnd = m_hwnd;

	set_windowDCmode_BeginPaint();
	m_isSetWindowDC = FALSE;
	m_isSetObjectMemoryDC = FALSE;
	m_isSetWindowBufferDC = FALSE;

	bitmapManager.initialize(hInstance);
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
	release_windowBufferDC();
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
	return m_isSetObjectMemoryDC;
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
	if (m_isSetObjectMemoryDC) return;

	if (!isCreatedMemoryDC)
	{
		//ȭ�� DC�� ȣȯ�� �Ǵ� memDC�� ����
		hDC.memory.windowBuffer = CreateCompatibleDC(hDC.window);
		//ȭ�� DCȭ ȣȯ�Ǵ� memDC�� ȣȯ�Ǵ� memory DC ����
		hDC.memory.background = CreateCompatibleDC(hDC.memory.windowBuffer);
		hDC.memory.ball = CreateCompatibleDC(hDC.memory.windowBuffer);
		hDC.memory.ball_mask = CreateCompatibleDC(hDC.memory.windowBuffer);

		//������ memory DC�鿡 hBitmap�� ���ý�Ŵ
		hBitmap.holdingOld.background
			= (HBITMAP)SelectObject(hDC.memory.background, hBitmap.resource.background);
		hBitmap.holdingOld.ball
			= (HBITMAP)SelectObject(hDC.memory.ball, hBitmap.resource.ball);
		hBitmap.holdingOld.ball_mask
			= (HBITMAP)SelectObject(hDC.memory.ball_mask, hBitmap.resource.ball_mask);
		
		isCreatedMemoryDC = TRUE;
		m_isSetObjectMemoryDC = TRUE;
	}

	//ȭ�� DC�� ȣȯ�Ǵ� hBitmap�� �ε��Ѵ�
	GetClientRect(hwnd, &windowRect);
	hBitmap.hDCwindowCompatible
		= CreateCompatibleBitmap(hDC.window, windowRect.right, windowRect.bottom);

	//ȭ�� DC�� ȣȯ�Ǵ� hBitmap�� windowBuffer memDC�� ���ý�Ų��
	hBitmap.holdingOld.windowBuffer
		= (HBITMAP)SelectObject(hDC.memory.windowBuffer, hBitmap.hDCwindowCompatible);

	m_isSetWindowBufferDC = TRUE;;
}
void PaintManager::release_ObjectMemoryDC()
{
	if (!m_isSetObjectMemoryDC) return;
	//holding�� ����� �� memoryDC�� �⺻ hBitmap�� ������
	//SelectObject(hDC.memory.windowBuffer, hBitmap.holdingOld.windowBuffer);
	SelectObject(hDC.memory.background, hBitmap.holdingOld.background);
	SelectObject(hDC.memory.ball, hBitmap.holdingOld.ball);
	SelectObject(hDC.memory.ball_mask, hBitmap.holdingOld.ball_mask);

	//memDC���� ������
	//DeleteDC(hDC.memory.windowBuffer);
	DeleteDC(hDC.memory.background);
	DeleteDC(hDC.memory.ball);
	DeleteDC(hDC.memory.ball_mask);

	//hBitmap�� ������
	DeleteObject(hBitmap.hDCwindowCompatible);

	m_isSetObjectMemoryDC = FALSE;
}
void PaintManager::release_windowBufferDC()
{
	if (!m_isSetWindowBufferDC) return;

	SelectObject(hDC.memory.windowBuffer, hBitmap.holdingOld.windowBuffer);
	DeleteDC(hDC.memory.windowBuffer);

	m_isSetWindowBufferDC = FALSE;
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
			BitBlt(hDC.memory.windowBuffer, i * 256, j * 256, 256, 256, hDC.memory.background, 0, 0, SRCCOPY);
}
void PaintManager::paint_ball_tobuffer(int x, int y)
{
	if (!isAbleToPrint()) return;

	BitBlt(
		hDC.memory.windowBuffer,
		x - BallSizeType / 2, y - BallSizeType / 2,
		BallSizeType, BallSizeType,
		hDC.memory.ball_mask, 0, 0, SRCAND
	);

	BitBlt(
		hDC.memory.windowBuffer,
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
		hDC.memory.windowBuffer, 0, 0,
		SRCCOPY
	);
}