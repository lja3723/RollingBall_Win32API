#ifndef __device_input_h__
#define __device_input_h__

#include <Windows.h>
#include <cmath>

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
		HWND hwnd;

		static const int numofButtons = 3;
		static const int LButton = 0;
		static const int RButton = 1;
		static const int MButton = 2;
		static const int notDragging = -1;
		BOOL buttons[numofButtons];
		POINT dragStart[numofButtons];
		POINT prevDragging[numofButtons];
		POINT dragging[numofButtons];
		POINT dragEnd[numofButtons];

		POINT absolute;
		POINT relative;

		POINT current;

	private:
		POINT delta(POINT* p1, POINT* p2);
		void update_mousePos();
		void button_down(int btn);
		void button_up(int btn);
		void mouse_move(LPARAM lParam);

		BOOL isValid(POINT p);
		void invalidate(LPPOINT p);

		BOOL isButtonDown(int btn);
		BOOL isButtonClicked(int btn, POINT* clicked_pos);
		BOOL isButtonDragging(int btn, POINT* dragStart, POINT* prevDragging);

	public:
		void init(HWND m_hwnd);

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
		void init(HWND hwnd);
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);
	};
}


#endif