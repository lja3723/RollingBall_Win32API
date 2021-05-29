#ifndef __device_output_h__
#define __device_output_h__

#include "monitor.h"

namespace RollingBall
{
	class Speaker
	{
	public:
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};


	class OutputDevice
	{
	public:
		Monitor monitor;
		Speaker speaker;
	public:
		//it must be called when program starts
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd);
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};
}


#endif