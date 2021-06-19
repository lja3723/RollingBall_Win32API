#include "paint_hbitmap.h"

using namespace RollingBall;

Bitmap Paint_hBitmap::m_bmp = Bitmap();

BOOL Paint_hBitmap::_windowBuffer::isSet()
{
	return m_windowBuffer != NULL;
}
void Paint_hBitmap::_windowBuffer::init()
{
	m_windowBuffer = NULL;
}
void RollingBall::Paint_hBitmap::_windowBuffer::set(const HWND& hwnd, const HDC& hDC_window)
{
	if (isSet())
		release();

	RECT windowRect;
	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(hwnd, &windowRect);
	m_windowBuffer = m_bmp.create_hDC_compatible(hDC_window, windowRect);
}
void RollingBall::Paint_hBitmap::_windowBuffer::release()
{
	if (!isSet()) return;
	//hBitmap을 삭제함
	m_bmp.delete_hDC_compatible(m_windowBuffer);
	init();
}

BOOL Paint_hBitmap::_windowBuffer::_old::isBackedUp()
{
	return m_windowBuffer != NULL;
}
void Paint_hBitmap::_windowBuffer::_old::init()
{
	m_windowBuffer = NULL;
}
//틀린코드 수정필요
void Paint_hBitmap::_windowBuffer::_old::backup(Paint_hDC& hDC)
{
	if (!hDC.mem.windowBuffer.isSet()) return;
	if (isBackedUp())
		rollback(hDC);

	m_windowBuffer
		= (HBITMAP)SelectObject(
			hDC.mem.windowBuffer,
			//잘못된 코드(_res의 m_windowBuffer를 인자로 줘야함)
			m_windowBuffer
		);
}
void Paint_hBitmap::_windowBuffer::_old::rollback(Paint_hDC& hDC)
{
	if (!isBackedUp()) return;
	SelectObject(
		hDC.mem.windowBuffer,
		m_windowBuffer
	);
	init();
}

BOOL Paint_hBitmap::_res::isSet()
{
	return flag_isSet;
}
void Paint_hBitmap::_res::init()
{
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;

	//아래 플래그 변수는 삭제할 수 있을 것 같다 
	//(resource.size() != 0)
	flag_isSet = FALSE;
}
void RollingBall::Paint_hBitmap::_res::set()
{
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = m_bmp(i);
	flag_isSet = TRUE;
}
void Paint_hBitmap::_res::resize(const size_t& newSize)
{
	m_res.resize(newSize);
}

BOOL Paint_hBitmap::_res::_old::isBackedUp()
{
	return flag_isBackedUp;
}
void Paint_hBitmap::_res::_old::init()
{
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;
}
//틀린코드 수정필요
void Paint_hBitmap::_res::_old::backup(Paint_hDC& hDC)
{
	if (!hDC.mem.res.isSet()) return;
	if (!isBackedUp())
		rollback(hDC);

	for (int i = 0; i < m_res.size(); i++)
		m_res[i]
		= (HBITMAP)SelectObject(
			hDC.mem.res[i],
			//아래 코드는 틀렸음(_res의 m_res를 인자로줘야함)
			m_res[i]
		);

	flag_isBackedUp = TRUE;
}
void Paint_hBitmap::_res::_old::rollback(Paint_hDC& hDC)
{
	if (!isBackedUp()) return;

	for (int i = 0; i < m_res.size(); i++)
		SelectObject(
			hDC.mem.res[i],
			m_res[i]
		);

	init();

	flag_isBackedUp = FALSE;
}
void Paint_hBitmap::_res::_old::resize(const size_t& newSize)
{
	m_res.resize(newSize);
}

BOOL RollingBall::Paint_hBitmap::init(HINSTANCE hInstance)
{
	return m_bmp.init(hInstance);
}
BOOL RollingBall::Paint_hBitmap::isInit()
{
	return m_bmp.isInit();
}
int RollingBall::Paint_hBitmap::bmpidx(Object& object, Scaler& scale, BOOL mask_texture)
{
	return m_bmp.idx(object, scale, mask_texture);
}
int RollingBall::Paint_hBitmap::res_count()
{
	if (!m_bmp.isInit()) return 0;
	return m_bmp.file_count();
}
void RollingBall::Paint_hBitmap::resize_vectors(const size_t& newSize)
{
	res.resize(newSize);
	res.old.resize(newSize);
}
