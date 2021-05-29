#include "device_output.h"

using namespace RollingBall;


void RollingBall::Speaker::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
}


BOOL RollingBall::OutputDevice::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	return monitor.init(m_hInstance, m_hwnd);
}

void RollingBall::OutputDevice::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	monitor.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	speaker.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
