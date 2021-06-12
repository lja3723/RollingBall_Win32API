#include "paint_hdc.h"

using namespace RollingBall;



/********************************
*
*		private functions
*		- hDCwindow Management
*
*********************************/

void Paint_hDC::_window::init(HWND hwnd)
{
	m_window = NULL;
	m_hwnd = hwnd;
}
void Paint_hDC::_window::set()
{
	if (isSet())
		release();

	if (mode.isGetDC())
		m_window = GetDC(m_hwnd);
	else
		m_window = BeginPaint(m_hwnd, &m_ps);
}
HDC& Paint_hDC::_window::operator()()
{
	return m_window;
}
void Paint_hDC::_window::release()
{
	if (!isSet()) return;

	if (mode.isGetDC())
		ReleaseDC(m_hwnd, m_window);
	else
		EndPaint(m_hwnd, &m_ps);

	init(m_hwnd);
}
BOOL Paint_hDC::_window::isSet()
{
	return m_window != NULL;
}

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


/********************************
*
*		private functions
*		- mem management
*
*********************************/
void Paint_hDC::_mem::_windowBuffer::init(Paint_hDC* hDC)
{
	m_hDC = hDC;
	m_windowBuffer = NULL;
}
void Paint_hDC::_mem::_windowBuffer::set()
{
	if (isSet())
		release();

	//화면 DC와 호환이 되는 memDC를 생성
	m_windowBuffer = CreateCompatibleDC(m_hDC->window());
}
HDC& Paint_hDC::_mem::_windowBuffer::operator()()
{
	return m_windowBuffer;
}
void Paint_hDC::_mem::_windowBuffer::release()
{
	if (!isSet()) return;

	DeleteDC(m_windowBuffer);
	init(m_hDC);
}
BOOL Paint_hDC::_mem::_windowBuffer::isSet()
{
	return m_windowBuffer != NULL;
}

void Paint_hDC::_mem::_res::init(Paint_hDC* hDC)
{
	m_hDC = hDC;
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;
}
void Paint_hDC::_mem::_res::set()
{
	if (isSet())
		release();

	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = CreateCompatibleDC(m_hDC->mem.windowBuffer());

	flag_isSet = TRUE;
}
HDC Paint_hDC::_mem::_res::operator()(int idx)
{
	if (0 <= idx && idx < m_res.size())
		return m_res[idx];
	else
		return NULL;
}
void Paint_hDC::_mem::_res::operator()(int idx, HDC m_hdc)
{
	if (0 <= idx && idx < m_res.size())
		m_res[idx] = m_hdc;
}
void Paint_hDC::_mem::_res::release()
{
	if (!isSet()) return;

	for (int i = 0; i < m_res.size(); i++)
		DeleteDC(m_res[i]);

	init(m_hDC);

	flag_isSet = FALSE;
}
void Paint_hDC::_mem::_res::resize(size_t newSize)
{
	m_res.resize(newSize);
}
BOOL Paint_hDC::_mem::_res::isSet()
{
	return flag_isSet;
}

void Paint_hDC::_mem::create()
{
	windowBuffer.set();
	res.set();
}
void Paint_hDC::_mem::del()
{
	windowBuffer.release();
	res.release();
}
void Paint_hDC::_mem::init(Paint_hDC* hDC)
{
	windowBuffer.init(hDC);
	res.init(hDC);
}

void Paint_hDC::init(HWND hwnd)
{
	window.init(hwnd);
	mem.init(this);
}
