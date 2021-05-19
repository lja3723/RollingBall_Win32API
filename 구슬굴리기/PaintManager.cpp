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
void PaintManager::init(HINSTANCE hInstance, HWND hwnd, int m_BallSizeType)
{
	winAPI.hInstance = hInstance;
	winAPI.hwnd = hwnd;
	memset(&winAPI.ps, 0, sizeof(winAPI.ps));
	memset(&winAPI.windowRect, 0, sizeof(winAPI.windowRect));

	hDCwindow_init();
	memoryDC_init();

	winAPI.hBitmap.windowBuffer = NULL;
	winAPI.hBitmap.res.background = NULL;
	winAPI.hBitmap.res.ball = NULL;
	winAPI.hBitmap.res.ball_mask = NULL;

	winAPI.hBitmap.old.windowBuffer = NULL;
	winAPI.hBitmap.old.res.background = NULL;
	winAPI.hBitmap.old.res.ball = NULL;
	winAPI.hBitmap.old.res.ball_mask = NULL;

	init_flags();
	init_bitmapManager();
	BallSizeType = m_BallSizeType;

	set_hBitmap_res(m_BallSizeType);

	flag.isInit = TRUE;
}
void PaintManager::init_bitmapManager()
{
	bitmapManager.init(winAPI.hInstance);
	flag.isInitBitmapManager = TRUE;
}

void PaintManager::beginPaint()
{
	hDCwindowMode_set_BeginPaint();
	hDCwindow_set();
	memoryDC_set();
}
void PaintManager::endPaint()
{
	flush_buffer();
	hDCwindow_release();
	memoryDC_release();
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
*		- initialization
*
*********************************/
void RollingBall::PaintManager::init_flags()
{
	hDCwindowMode_set_BeginPaint();
	flag.isSetHDCwindow = FALSE;
	flag.isSetMemoryDC = FALSE;
	flag.isInit = FALSE;
	flag.isInitBitmapManager = FALSE;
}


/********************************
*
*		private functions
*		- BOOL returns
*
*********************************/
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
	return flag.isSetHDCwindow;
}
BOOL PaintManager::isSetMemoryDC()
{
	return flag.isSetMemoryDC;
}
BOOL PaintManager::isInit()
{
	return flag.isInit;
}
BOOL PaintManager::isInitBitmapManager()
{
	return flag.isInitBitmapManager;
}
BOOL PaintManager::isReadyToPaint()
{
	if (!isSetHDCwindow()) return FALSE;
	if (!isSetMemoryDC()) return FALSE;
	return TRUE;
}


/********************************
*
*		private functions
*		- set windowDCmode
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


/********************************
*
*		private functions
*		- Manage hDCwindow
*
*********************************/
void PaintManager::hDCwindow_set()
{
	if (isSetHDCwindow())
	{
		hDCwindow_release();
		hDCwindow_init();
	}

	if (isHDCwindowMode_GetDC())
		winAPI.hDC.window = GetDC(winAPI.hwnd);
	else
		winAPI.hDC.window = BeginPaint(winAPI.hwnd, &winAPI.ps);

	flag.isSetHDCwindow = TRUE;
}
void PaintManager::hDCwindow_release()
{
	if (!isSetHDCwindow()) return;

	if (isHDCwindowMode_GetDC())
		ReleaseDC(winAPI.hwnd, winAPI.hDC.window);
	else
		EndPaint(winAPI.hwnd, &winAPI.ps);

	hDCwindow_init();

	flag.isSetHDCwindow = FALSE;
}
void PaintManager::hDCwindow_init()
{
	winAPI.hDC.window = NULL;
}


/********************************
*
*		private functions
*		- Manage memoryDC
*
*********************************/
void PaintManager::memoryDC_set()
{
	if (!isSetHDCwindow()) return;
	if (isSetMemoryDC()) return;

	//ȭ�� DC�� ȣȯ�� �Ǵ� memDC�� ����
	winAPI.hDC.mem.windowBuffer = CreateCompatibleDC(winAPI.hDC.window);
	//ȭ�� DCȭ ȣȯ�Ǵ� memDC�� ȣȯ�Ǵ� memory DC ����
	winAPI.hDC.mem.res.background = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);
	winAPI.hDC.mem.res.ball = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);
	winAPI.hDC.mem.res.ball_mask = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);

	//ȭ�� DC�� ȣȯ�Ǵ� hBitmap�� �ε��Ѵ�
	GetClientRect(winAPI.hwnd, &winAPI.windowRect);
	winAPI.hBitmap.windowBuffer
		= CreateCompatibleBitmap(winAPI.hDC.window, winAPI.windowRect.right, winAPI.windowRect.bottom);

	//������ memory DC�鿡 hBitmap�� ���ý�Ŵ
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
void PaintManager::memoryDC_release()
{
	if (!isSetMemoryDC()) return;
	//holding�� ����� �� memoryDC�� �⺻ hBitmap�� ������
	SelectObject(winAPI.hDC.mem.windowBuffer, winAPI.hBitmap.old.windowBuffer);
	SelectObject(winAPI.hDC.mem.res.background, winAPI.hBitmap.old.res.background);
	SelectObject(winAPI.hDC.mem.res.ball, winAPI.hBitmap.old.res.ball);
	SelectObject(winAPI.hDC.mem.res.ball_mask, winAPI.hBitmap.old.res.ball_mask);

	//memDC���� ������
	DeleteDC(winAPI.hDC.mem.windowBuffer);
	DeleteDC(winAPI.hDC.mem.res.background);
	DeleteDC(winAPI.hDC.mem.res.ball);
	DeleteDC(winAPI.hDC.mem.res.ball_mask);

	//memDC���� �ʱ�ȭ��
	memoryDC_init();

	//hBitmap�� ������
	DeleteObject(winAPI.hBitmap.windowBuffer);

	flag.isSetMemoryDC = FALSE;
}
void PaintManager::memoryDC_init()
{
	winAPI.hDC.mem.windowBuffer = NULL;
	winAPI.hDC.mem.res.background = NULL;
	winAPI.hDC.mem.res.ball = NULL;
	winAPI.hDC.mem.res.ball_mask = NULL;
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
void PaintManager::set_hBitmap_res(int BallSizeType)
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
	if (!isReadyToPaint()) return;

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
	if (!isReadyToPaint()) return;

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
	if (!isReadyToPaint()) return;

	BitBlt(
		winAPI.hDC.window, 0, 0, winAPI.windowRect.right, winAPI.windowRect.bottom,
		winAPI.hDC.mem.windowBuffer, 0, 0,
		SRCCOPY
	);
}