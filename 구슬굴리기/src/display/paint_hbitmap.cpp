#include "paint_hbitmap.h"

using namespace RollingBall;


BOOL Paint_hBitmap::_windowBuffer::_old::isBackedUp()
{
	return m_windowBuffer != NULL;
}
void Paint_hBitmap::_windowBuffer::_old::backup()
{
	if (!m_hDC->mem.windowBuffer.isSet()) return;
	if (isBackedUp())
		rollback();

	m_windowBuffer
		= (HBITMAP)SelectObject(
			m_hDC->mem.windowBuffer(),
			m_hBitmap->windowBuffer()
		);
}
void Paint_hBitmap::_windowBuffer::_old::rollback()
{
	if (!isBackedUp()) return;
	SelectObject(
		m_hDC->mem.windowBuffer(),
		m_windowBuffer
	);
	init(m_hDC, m_hBitmap);
}
void Paint_hBitmap::_windowBuffer::_old::init(Paint_hDC* hDC, Paint_hBitmap* hBitmap)
{
	m_windowBuffer = NULL;
	m_hDC = hDC;
	m_hBitmap = hBitmap;
}


BOOL Paint_hBitmap::_windowBuffer::isSet()
{
	return m_windowBuffer != NULL;
}
void Paint_hBitmap::_windowBuffer::set()
{
	if (isSet())
		release();

	//화면 DC와 호환되는 hBitmap을 로드한다
	GetClientRect(m_hwnd, &windowRect);
	m_windowBuffer = m_bmp->create_hDC_compatible(m_hDC->window(), windowRect);
}
void Paint_hBitmap::_windowBuffer::release()
{
	if (!isSet()) return;
	//hBitmap을 삭제함
	m_bmp->delete_hDC_compatible(m_windowBuffer);
	init(m_hwnd, m_bmp, m_hDC, m_hBitmap);
}
const HBITMAP& Paint_hBitmap::_windowBuffer::operator()()
{
	return m_windowBuffer;
}
void Paint_hBitmap::_windowBuffer::init(HWND hwnd, Bitmap* bmp, Paint_hDC* hDC, Paint_hBitmap* hBitmap)
{
	m_windowBuffer = NULL;
	m_hwnd = hwnd;
	m_bmp = bmp;
	m_hDC = hDC;
	m_hBitmap = hBitmap;
	old.init(hDC, hBitmap);
}


BOOL Paint_hBitmap::_res::_old::isBackedUp()
{
	return flag_isBackedUp;
}
void Paint_hBitmap::_res::_old::backup()
{
	if (!m_hDC->mem.res.isSet()) return;
	if (!isBackedUp())
		rollback();

	for (int i = 0; i < m_res.size(); i++)
		m_res[i]
		= (HBITMAP)SelectObject(
			m_hDC->mem.res(i),
			m_res_outer->m_res[i]
		);

	flag_isBackedUp = TRUE;
}
void Paint_hBitmap::_res::_old::rollback()
{
	if (!isBackedUp()) return;

	for (int i = 0; i < m_res.size(); i++)
		SelectObject(
			m_hDC->mem.res(i),
			m_res[i]
		);

	init(m_res_outer, m_hDC);

	flag_isBackedUp = FALSE;
}
void Paint_hBitmap::_res::_old::resize(size_t newSize)
{
	m_res.resize(newSize);
}
void Paint_hBitmap::_res::_old::init(_res* res_outer, Paint_hDC* hDC)
{
	m_res_outer = res_outer;
	m_hDC = hDC;
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;
}


BOOL Paint_hBitmap::_res::isSet()
{
	return flag_isSet;
}
void Paint_hBitmap::_res::set()
{
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = (*m_bmp)(i);

	flag_isSet = TRUE;
}
void Paint_hBitmap::_res::init(Paint_hDC* hDC, Bitmap* bmp)
{
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;

	//아래 플래그 변수는 삭제할 수 있을 것 같다 
	//(winAPI.hBitmap.resource.size() != 0)
	flag_isSet = FALSE;
}
void Paint_hBitmap::_res::resize(size_t newSize)
{
	m_res.resize(newSize);
}


void Paint_hBitmap::init(HWND hwnd, Bitmap* bmp, Paint_hDC* hDC)
{
	if (flag_isInit) return;

	windowBuffer.init(hwnd, bmp, hDC, this);
	res.init(hDC, bmp);

	windowBuffer.old.init(hDC, this);
	res.old.init(&res, hDC);

	res.set();

	flag_isInit = TRUE;
}
BOOL Paint_hBitmap::isInit()
{
	return flag_isInit;
}
