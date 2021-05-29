#ifndef __device_h__
#define __device_h__

#include "device_input.h"
#include "device_output.h"

namespace RollingBall
{
	class Device
	{
	public:
		InputDevice in;
		OutputDevice out;

	public:
		//it must be called when program starts
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd);
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};
}


#endif