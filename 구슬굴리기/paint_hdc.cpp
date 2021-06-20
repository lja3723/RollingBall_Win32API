#include "paint_hdc.h"

using namespace RollingBall;



//////////////////////////////////////////
// 
//		hDC.window manage
// 
//////////////////////////////////////////
void Paint_hDC::_window::_mode::set_BeginPaint()
{
	flag_isGetDC = FALSE;
}
void Paint_hDC::_window::_mode::set_GetDC()
{
	flag_isGetDC = TRUE;
}
BOOL Paint_hDC::_window::_mode::isGetDC()
{
	return flag_isGetDC;
}
BOOL Paint_hDC::_window::_mode::isBeginPaint()
{
	return !isGetDC();
}

void Paint_hDC::_window::init()
{
	m_window = NULL;
	memset(&m_ps, 0, sizeof(m_ps));
}
void Paint_hDC::_window::set(HWND hwnd)
{
	if (isSet())
		release(hwnd);

	if (mode.isGetDC())
		m_window = GetDC(hwnd);
	else
		m_window = BeginPaint(hwnd, &m_ps);
}
void Paint_hDC::_window::release(const HWND& hwnd)
{
	if (!isSet()) return;

	if (mode.isGetDC())
		ReleaseDC(hwnd, m_window);
	else
		EndPaint(hwnd, &m_ps);

	init();
}
BOOL Paint_hDC::_window::isSet()
{
	return m_window != NULL;
}
Paint_hDC::_window::operator const HDC& ()
{
	return m_window;
}



//////////////////////////////////////////
// 
//		hDC.mem.windowBuffer manage
// 
//////////////////////////////////////////
void Paint_hDC::_mem::_windowBuffer::init()
{
	m_windowBuffer = NULL;
}
void Paint_hDC::_mem::_windowBuffer::set(const HDC& window)
{
	if (isSet())
		release();

	//화면 DC와 호환이 되는 memDC를 생성
	m_windowBuffer = CreateCompatibleDC(window);
}
void Paint_hDC::_mem::_windowBuffer::release()
{
	if (!isSet()) return;

	DeleteDC(m_windowBuffer);
	init();
}
BOOL Paint_hDC::_mem::_windowBuffer::isSet()
{
	return m_windowBuffer != NULL;
}
void Paint_hDC::_mem::_windowBuffer::select(Paint_hBitmap& hBitmap)
{
	if (!isSet()) return;
	if (hBitmap.windowBuffer.isBackedUp())
		restore(hBitmap);

	hBitmap.windowBuffer.old
		= (HBITMAP)SelectObject(
			m_windowBuffer,
			hBitmap.windowBuffer
		);
}
void Paint_hDC::_mem::_windowBuffer::restore(Paint_hBitmap& hBitmap)
{
	if (!hBitmap.windowBuffer.isBackedUp()) return;
	SelectObject(
		m_windowBuffer,
		hBitmap.windowBuffer.old
	);
}
Paint_hDC::_mem::_windowBuffer::operator const HDC& ()
{
	return m_windowBuffer;
}



//////////////////////////////////////////
// 
//		hDC.mem.res manage
// 
//////////////////////////////////////////
void Paint_hDC::_mem::_res::init()
{
	flag_isSet = NULL;
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;
}
void Paint_hDC::_mem::_res::set(const HDC& mem_windowBuffer)
{
	if (isSet())
		release();

	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = CreateCompatibleDC(mem_windowBuffer);

	flag_isSet = TRUE;
}
void Paint_hDC::_mem::_res::release()
{
	if (!isSet()) return;

	for (int i = 0; i < m_res.size(); i++)
		DeleteDC(m_res[i]);

	init();

	flag_isSet = FALSE;
}
void Paint_hDC::_mem::_res::resize(const size_t& newSize)
{
	m_res.resize(newSize);
}
BOOL Paint_hDC::_mem::_res::isSet()
{
	return flag_isSet;
}
void Paint_hDC::_mem::_res::select(Paint_hBitmap& hBitmap)
{
	if (!isSet()) return;
	if (!hBitmap.res.isBackedUp())
		restore(hBitmap);

	for (int i = 0; i < hBitmap.res.size(); i++)
		hBitmap.res.old(i)
		= (HBITMAP)SelectObject(
			m_res[i],
			hBitmap.res(i)
		);

	hBitmap.res.isBackedUp(TRUE);
}
void Paint_hDC::_mem::_res::restore(Paint_hBitmap& hBitmap)
{
	if (!hBitmap.res.isBackedUp()) return;

	for (int i = 0; i < m_res.size(); i++)
		SelectObject(
			m_res[i],
			hBitmap.res.old(i)
		);

	hBitmap.res.isBackedUp(FALSE);
}
const HDC& Paint_hDC::_mem::_res::operator()(int idx)
{
	if (0 <= idx && idx < m_res.size())
		return m_res[idx];
	else
		return m_res[0];
}



//////////////////////////////////////////
// 
//		hDC.mem || hDC manage
// 
//////////////////////////////////////////
void Paint_hDC::_mem::set(const HDC& window, const HDC& mem_windowBuffer)
{
	windowBuffer.set(window);
	res.set(mem_windowBuffer);
}
void Paint_hDC::_mem::release()
{
	windowBuffer.release();
	res.release();
}

void Paint_hDC::resize_res_vector(const size_t& newSize)
{
	mem.res.resize(newSize);
}
