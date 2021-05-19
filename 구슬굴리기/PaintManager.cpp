#include "PaintManager.h"
using namespace RollingBall;



/********************************
*
*		static variables
*		initialization
*
*********************************/
int PaintManager::doublebuff_count = 0;



/********************************
* 
*		public functions
*
*********************************/
PaintManager::~PaintManager()
{

}
void PaintManager::init(HINSTANCE hInstance, HWND hwnd, int BallSizeType)
{
	winAPI.hInstance = hInstance;
	winAPI.hwnd = hwnd;
	memset(&winAPI.ps, 0, sizeof(winAPI.ps));
	memset(&winAPI.windowRect, 0, sizeof(winAPI.windowRect));

	hDCwindow_init();
	memDC_windowBuffer_init();
	memDC_res_init();

	hBitmap_windowBuffer_init();
	hBitmap_res_init();

	hBitmap_old_windowBuffer_init();
	hBitmap_old_res_init();

	init_flags();
	init_bitmapManager();

	set_BallSizeType(BallSizeType);
	hBitmap_res_set();

	flag.isInit = TRUE;
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
//ó�� init �� �� �� ���� ȣ��Ǵ� �Լ�
void PaintManager::init_flags()
{
	hDCwindowMode_set_BeginPaint();

	flag.isSetMemDCres = FALSE;
	flag.isSetHBitmapRes = FALSE;
	flag.isBackedUpHBitmapRes = FALSE;

	flag.isDoubleBufferingStart = FALSE;
	flag.isInit = FALSE;
	flag.isInitBitmapManager = FALSE;
}
void PaintManager::init_bitmapManager()
{
	bitmapManager.init(winAPI.hInstance);
	flag.isInitBitmapManager = TRUE;
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
BOOL PaintManager::isInitBitmapManager()
{
	return flag.isInitBitmapManager;
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
	
	//ȭ�� DC�� ȣȯ�� �Ǵ� memDC�� ����
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
	winAPI.hDC.mem.res.background = NULL;
	winAPI.hDC.mem.res.ball = NULL;
	winAPI.hDC.mem.res.ball_mask = NULL;
}
void PaintManager::memDC_res_set()
{
	if (isSetMemDCres())
		memDC_res_release();

	//ȭ�� DCȭ ȣȯ�Ǵ� memDC�� ȣȯ�Ǵ� memory DC ����
	winAPI.hDC.mem.res.background = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);
	winAPI.hDC.mem.res.ball = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);
	winAPI.hDC.mem.res.ball_mask = CreateCompatibleDC(winAPI.hDC.mem.windowBuffer);

	flag.isSetMemDCres = TRUE;
}
void PaintManager::memDC_res_release()
{
	if (!isSetMemDCres()) return;

	DeleteDC(winAPI.hDC.mem.res.background);
	DeleteDC(winAPI.hDC.mem.res.ball);
	DeleteDC(winAPI.hDC.mem.res.ball_mask);
	memDC_res_init();

	flag.isSetMemDCres = FALSE;
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

	//ȭ�� DC�� ȣȯ�Ǵ� hBitmap�� �ε��Ѵ�
	GetClientRect(winAPI.hwnd, &winAPI.windowRect);
	winAPI.hBitmap.windowBuffer
		= CreateCompatibleBitmap(winAPI.hDC.window, winAPI.windowRect.right, winAPI.windowRect.bottom);
}
void PaintManager::hBitmap_windowBuffer_release()
{
	if (!isSetHBitmapWindowBuffer()) return;
	//hBitmap�� ������
	DeleteObject(winAPI.hBitmap.windowBuffer);
	hBitmap_windowBuffer_init();
}
void PaintManager::hBitmap_res_init()
{
	winAPI.hBitmap.res.background = NULL;
	winAPI.hBitmap.res.ball = NULL;
	winAPI.hBitmap.res.ball_mask = NULL;

	flag.isSetHBitmapRes = FALSE;
}
void PaintManager::hBitmap_res_set()
{
	winAPI.hBitmap.res.background = bitmapManager.get_hBitmap_floor();
	winAPI.hBitmap.res.ball = bitmapManager.get_hBitmap_ball();
	winAPI.hBitmap.res.ball_mask = bitmapManager.get_hBitmap_ball_mask();

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
	winAPI.hBitmap.old.res.background = NULL;
	winAPI.hBitmap.old.res.ball = NULL;
	winAPI.hBitmap.old.res.ball_mask = NULL;
}
void PaintManager::hBitmap_old_res_backup()
{
	if (!isSetMemDCres()) return;
	if (!isBackedUpHBitmapRes())
		hBitmap_old_res_rollback();

	winAPI.hBitmap.old.res.background
		= (HBITMAP)SelectObject(winAPI.hDC.mem.res.background, winAPI.hBitmap.res.background);
	winAPI.hBitmap.old.res.ball
		= (HBITMAP)SelectObject(winAPI.hDC.mem.res.ball, winAPI.hBitmap.res.ball);
	winAPI.hBitmap.old.res.ball_mask
		= (HBITMAP)SelectObject(winAPI.hDC.mem.res.ball_mask, winAPI.hBitmap.res.ball_mask);

	flag.isBackedUpHBitmapRes = TRUE;
}
void PaintManager::hBitmap_old_res_rollback()
{
	if (!isBackedUpHBitmapRes()) return;

	SelectObject(winAPI.hDC.mem.res.background, winAPI.hBitmap.old.res.background);
	SelectObject(winAPI.hDC.mem.res.ball, winAPI.hBitmap.old.res.ball);
	SelectObject(winAPI.hDC.mem.res.ball_mask, winAPI.hBitmap.old.res.ball_mask);
	hBitmap_old_res_init();

	flag.isBackedUpHBitmapRes = FALSE;
}



/********************************
*
*		private functions
*		- double buffering manage
*
*********************************/
void PaintManager::doubleBuffering_start()
{
	if (isDoubleBufferingStart()) return;
	if (!isSetHDCwindow()) return;

	//hBitmap.windowBuffer�� ������
	hBitmap_windowBuffer_set();

	if (doublebuff_count == 0)
	{
		//memDC���� ������
		memDC_windowBuffer_set();
		memDC_res_set();

		//������ memory DC�鿡 hBitmap�� ���ý�Ű�鼭 hBitmap.old�� ���
		hBitmap_old_windowBuffer_backup();
		hBitmap_old_res_backup();
	}

	flag.isDoubleBufferingStart = TRUE;
}
void PaintManager::doubleBuffering_stop()
{
	if (!isDoubleBufferingStart()) return;

	//holding�� ����� �� memoryDC�� �⺻ hBitmap�� ������

	//memDC���� release��
	memDC_windowBuffer_release();
	memDC_res_release();

	//hBitmap.windowBuffer�� release��
	hBitmap_windowBuffer_release();

	flag.isDoubleBufferingStart = FALSE;
}



/********************************
*
*		private functions
*		- variable management
*
*********************************/
void PaintManager::set_BallSizeType(int m_BallSizeType)
{
	bitmapManager.set_BallSizeType(m_BallSizeType);
}



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
				winAPI.hDC.mem.res.background, 0, 0,
				SRCCOPY
			);
}
void PaintManager::paint_background_ruller_tobuffer()
{

}
void PaintManager::paint_ball_tobuffer(int x, int y)
{
	if (!isReadyToPaint()) return;
	int ballType = bitmapManager.get_BallSizeType();

	BitBlt(
		winAPI.hDC.mem.windowBuffer,
		x - ballType / 2, y - ballType / 2, ballType, ballType,
		winAPI.hDC.mem.res.ball_mask, 0, 0,
		SRCAND
	);

	BitBlt(
		winAPI.hDC.mem.windowBuffer,
		x - ballType / 2, y - ballType / 2, ballType, ballType,
		winAPI.hDC.mem.res.ball, 0, 0,
		SRCPAINT
	);
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


