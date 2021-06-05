#include "paint_hdc.h"

using namespace RollingBall;





/********************************
*
*		private functions
*		- BOOL returns
*
*********************************/
BOOL Paint_hDC::_isWindowMode::GetDC()
{
	return flag_GetDC;
}
BOOL Paint_hDC::_isWindowMode::BeginPaint()
{
	return !GetDC();
}

BOOL Paint_hDC::_isSet::window()
{
	return hDC->m_window != NULL;
}
BOOL Paint_hDC::_isSet::memWindowBuffer()
{
	return hDC->mem.m_mem.windowBuffer != NULL;
}
BOOL Paint_hDC::_isSet::memRes()
{
	return flag_MemDCres;
}


/********************************
*
*		private functions
*		- hDCwindow Management
*
*********************************/
void Paint_hDC::_windowMode::set_BeginPaint()
{
	hDC->isWindowMode.flag_GetDC = FALSE;
}
void Paint_hDC::_windowMode::set_GetDC()
{
	hDC->isWindowMode.flag_GetDC = TRUE;
}

void Paint_hDC::_window::init()
{
	hDC->m_window = NULL;
}
void Paint_hDC::_window::set()
{
	if (hDC->isSet.window())
		release();

	if (hDC->isWindowMode.GetDC())
		hDC->m_window = GetDC(hDC->hwnd);
	else
		hDC->m_window = BeginPaint(hDC->hwnd, &hDC->ps);
}
HDC& RollingBall::Paint_hDC::_window::get()
{
	return hDC->m_window;
}
void Paint_hDC::_window::release()
{
	if (!hDC->isSet.window()) return;

	if (hDC->isWindowMode.GetDC())
		ReleaseDC(hDC->hwnd, hDC->m_window);
	else
		EndPaint(hDC->hwnd, &hDC->ps);

	init();
}




/********************************
*
*		private functions
*		- mem management
*
*********************************/
void Paint_hDC::_mem::_windowBuffer::init()
{
	memDC->m_mem.windowBuffer = NULL;
}
void Paint_hDC::_mem::_windowBuffer::set()
{
	if (hDC->isSet.memWindowBuffer())
		release();

	//화면 DC와 호환이 되는 memDC를 생성
	memDC->m_mem.windowBuffer = CreateCompatibleDC(hDC->m_window);
}
HDC& RollingBall::Paint_hDC::_mem::_windowBuffer::get()
{
	return memDC->m_mem.windowBuffer;
}
void Paint_hDC::_mem::_windowBuffer::release()
{
	if (!hDC->isSet.memWindowBuffer()) return;

	DeleteDC(memDC->m_mem.windowBuffer);
	init();
}

void Paint_hDC::_mem::_res::init()
{
	for (int i = 0; i < hDC->res_count; i++)
		memDC->m_mem.res[i] = NULL;
}
void Paint_hDC::_mem::_res::set()
{
	if (hDC->isSet.memRes())
		release();

	//화면 DC화 호환되는 memDC와 호환되는 memory DC 생성
	for (int i = 0; i < hDC->res_count; i++)
		memDC->m_mem.res[i] = CreateCompatibleDC(memDC->m_mem.windowBuffer);

	hDC->isSet.flag_MemDCres = TRUE;
}
vector<HDC>& RollingBall::Paint_hDC::_mem::_res::get()
{
	return hDC->mem.m_mem.res;
}
void Paint_hDC::_mem::_res::release()
{
	if (!hDC->isSet.memRes()) return;

	for (int i = 0; i < hDC->res_count; i++)
		DeleteDC(memDC->m_mem.res[i]);

	init();

	hDC->isSet.flag_MemDCres = FALSE;
}

void RollingBall::Paint_hDC::_mem::_res::resize(size_t newSize)
{
	memDC->m_mem.res.resize(newSize);
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

void RollingBall::Paint_hDC::init(HWND m_hwnd, int m_res_count)
{
	hwnd = m_hwnd;
	res_count = m_res_count;
	window.init();
	mem.windowBuffer.init();
	mem.res.init();
}
