#ifndef __device_input_h__
#define __device_input_h__

#include <Windows.h>

namespace RollingBall
{
	class Keyboard
	{
	private:
		static const int numofKeys = 256;
		BOOL keys[numofKeys];

		void key_down(WPARAM VK_msg);
		void key_up(WPARAM VK_msg);
	public:
		void init();
		BOOL isKeyDown(WPARAM VK_msg);
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};

	class Mouse
	{
	private:
		//0: LButton
		//1: RButton
		//2: MButton
		static const int numofButtons = 3;
		static const int notDragging = -1;
		BOOL buttons[numofButtons];
		struct PixelCoord {
			int x;
			int y;
		};
		PixelCoord current;
		PixelCoord dragStart[numofButtons];
		PixelCoord dragEnd[numofButtons];

	private:
		void button_down(int btn);
		void button_up(int btn);
		void mouse_move(LPARAM lParam);

	public:
		void init();

		BOOL isLButtonDown();
		BOOL isRButtonDown();
		BOOL isMButtonDown();

		BOOL isLButtonClicked();
		BOOL isRButtonClicked();
		BOOL isMButtonClicked();

		BOOL isLButtonDragging();
		BOOL isRButtonDragging();
		BOOL isMButtonDragging();

		BOOL isWheelUp();
		BOOL isWheelDown();
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
		int x();
		int y();
	};

	class InputDevice
	{
	public:
		Keyboard keyboard;
		Mouse mouse;

	public:
		//it must be called when program starts
		void init();
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};
}


#endif