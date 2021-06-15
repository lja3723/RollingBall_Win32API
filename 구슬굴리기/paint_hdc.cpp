#include "paint_hdc.h"

using namespace RollingBall;




/********************************
*
*		private functions
*		- hDCwindow Management
*
*********************************/
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
RollingBall::Paint_hDC::_window::operator const HDC&()
{
	return m_window;
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
RollingBall::Paint_hDC::_mem::_windowBuffer::operator const HDC& ()
{
	return m_windowBuffer;
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
const HDC& Paint_hDC::_mem::_res::operator[](int idx)
{
	if (0 <= idx && idx < m_res.size())
		return m_res[idx];
	else
		return NULL;

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

void Paint_hDC::_mem::create(const HDC& window, const HDC& mem_windowBuffer)
{
	windowBuffer.set(window);
	res.set(mem_windowBuffer);
}
void Paint_hDC::_mem::del()
{
	windowBuffer.release();
	res.release();
}
