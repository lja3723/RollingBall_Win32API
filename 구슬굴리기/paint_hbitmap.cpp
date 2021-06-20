#include "paint_hbitmap.h"

using namespace RollingBall;

Bitmap Paint_hBitmap::m_bmp = Bitmap();

//아래 네 개의 함수는 Paint_hDC로 옮길 필요 있음
void Paint_hBitmap::_windowBuffer::backup(Paint_hDC& hDC)
{
	if (!hDC.mem.windowBuffer.isSet()) return;
	if (old.isBackedUp())
		rollback(hDC);

	old.m_windowBuffer
		= (HBITMAP)SelectObject(
			hDC.mem.windowBuffer,
			m_windowBuffer
		);
}
void Paint_hBitmap::_windowBuffer::rollback(Paint_hDC& hDC)
{
	if (!old.isBackedUp()) return;
	SelectObject(
		hDC.mem.windowBuffer,
		old.m_windowBuffer
	);
}
void Paint_hBitmap::_res::backup(Paint_hDC& hDC)
{
	if (!hDC.mem.res.isSet()) return;
	if (!old.isBackedUp())
		rollback(hDC);

	for (int i = 0; i < old.m_res.size(); i++)
		old.m_res[i]
		= (HBITMAP)SelectObject(
			hDC.mem.res[i],
			m_res[i]
		);

	old.flag_isBackedUp = TRUE;
}
void Paint_hBitmap::_res::rollback(Paint_hDC& hDC)
{
	if (!old.isBackedUp()) return;

	for (int i = 0; i < m_res.size(); i++)
		SelectObject(
			hDC.mem.res[i],
			old.m_res[i]
		);

	old.flag_isBackedUp = FALSE;
}



//////////////////////////////////////////
// 
//		hBitmap.windowBuffer manage
// 
//////////////////////////////////////////
BOOL Paint_hBitmap::_windowBuffer::_old::isBackedUp()
{
	return m_windowBuffer != NULL;
}
void Paint_hBitmap::_windowBuffer::_old::init()
{
	m_windowBuffer = NULL;
}
BOOL Paint_hBitmap::_windowBuffer::isSet()
{
	return m_windowBuffer != NULL;
}
void Paint_hBitmap::_windowBuffer::init()
{
	m_windowBuffer = NULL;
}
//void Paint_hBitmap::_windowBuffer::set(RECT& windowRect, Paint_hDC& hDC)
void Paint_hBitmap::_windowBuffer::set(RECT& windowRect, const HDC& window)
{
	if (isSet())
		//release(hDC);
		release();

	//화면 DC와 호환되고 크기가 windowRect인 hBitmap을 생성
	m_windowBuffer = m_bmp.create_hDC_compatible(window, windowRect);

	//화면 DC 호환 hBitmap을 hDC.mem.windowBuffer에 선택시킴
	//동시에 기존의 hBitmap을 old hBitmap에 백업시킨다
	//backup(hDC);
}
//void Paint_hBitmap::_windowBuffer::release(Paint_hDC& hDC)
void Paint_hBitmap::_windowBuffer::release()
{
	if (!isSet()) return;

	//rollback(hDC);

	//hBitmap을 삭제함
	m_bmp.delete_hDC_compatible(m_windowBuffer);
	init();
}



//////////////////////////////////////
// 
//		hBitmap.res manage
// 
//////////////////////////////////////
BOOL Paint_hBitmap::_res::_old::isBackedUp()
{
	return flag_isBackedUp;
}
void Paint_hBitmap::_res::_old::init()
{
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;
}
void Paint_hBitmap::_res::_old::resize(const size_t& newSize)
{
	m_res.resize(newSize);
}
BOOL Paint_hBitmap::_res::isSet()
{
	return flag_isSet;
}
void Paint_hBitmap::_res::init()
{
	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = NULL;

	flag_isSet = FALSE;
}
//void Paint_hBitmap::_res::set(Paint_hDC& hDC)
void Paint_hBitmap::_res::set()
{
	if (isSet())
		//release(hDC);
		release();

	for (int i = 0; i < m_res.size(); i++)
		m_res[i] = m_bmp(i);

	//backup(hDC);

	flag_isSet = TRUE;
}
//void Paint_hBitmap::_res::release(Paint_hDC& hDC)
void Paint_hBitmap::_res::release()
{
	if (!isSet()) return;

	//rollback(hDC);
	init();
}
void Paint_hBitmap::_res::resize(const size_t& newSize)
{
	m_res.resize(newSize);
	old.m_res.resize(newSize);
}



//////////////////////////////////////
// 
//		hBitmap public interface
// 
//////////////////////////////////////
BOOL Paint_hBitmap::init(HINSTANCE hInstance)
{
	return m_bmp.init(hInstance);
}
BOOL Paint_hBitmap::isInit()
{
	return m_bmp.isInit();
}
int Paint_hBitmap::bmpidx(Object& object, Scaler& scale, BOOL mask_texture)
{
	return m_bmp.idx(object, scale, mask_texture);
}
int Paint_hBitmap::res_count()
{
	if (!m_bmp.isInit()) return 0;
	return m_bmp.file_count();
}
void Paint_hBitmap::resize_res_vector(const size_t& newSize)
{
	res.resize(newSize);
}
