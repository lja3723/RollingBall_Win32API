#include "device.h"

using namespace RollingBall;

BOOL RollingBall::Device::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	if (!out.init(m_hInstance, m_hwnd))
		return FALSE;
	in.init(m_hwnd);

	return TRUE;
}

void RollingBall::Device::translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam)
{
	in.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
	out.translate_windowEvent(m_iMsg, m_wParam, m_lParam);
}
