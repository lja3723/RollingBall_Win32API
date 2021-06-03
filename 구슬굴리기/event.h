#ifndef __event_h__
#define __event_h__

#include <vector>
#include <Windows.h>

using std::vector;

namespace RollingBall
{
	/*
	* WM 메세지를 재해석해 사용하기 편하게 가공한 이벤트 추상 객체
	*/
	class Event abstract
	{

	};



	/*
	* 마우스 이벤트가 발생하면 그와 관련된 정보를 전달할 이벤트 객체
	*/
	class MouseEvent : Event
	{
	public:
		POINT pos;
	};



	/*
	* 키보드 이벤트가 발생하면 그와 관련된 정보를 전달할 이벤트 객체
	*/
	class KeyboardEvent : Event
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



	/*
	* WinAPI의 WM 메세지를 이벤트 객체로 해석한 뒤,
	* 이벤트 수용가능 객체에 이벤트를 전달하는 클래스
	*/
	class EventProducer
	{
	private:
		static MouseEvent produce_mouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam);
		static KeyboardEvent produce_keyboardEvent(UINT iMsg, WPARAM wParam, LPARAM lParam);
	public:
		static void translate_windowEvent(UINT iMsg, WPARAM wParam, LPARAM lParam);
	};



	/*
	* 이벤트를 처리할 수 있는 능력을 부여하는 인터페이스 클래스
	* 이벤트를 처리하기 위해서는 가상 함수들을 반드시 구현해야 함
	*/
	class EventAcceptable
	{
		friend class EventProducer;
	private:
		static int object_count;
		static vector<EventAcceptable*> object_ref;
	protected:

	public:
		EventAcceptable();
		~EventAcceptable();

		virtual void event_mouse(MouseEvent e) abstract;
		virtual void event_keyboard(KeyboardEvent e) abstract;
		virtual BOOL isObjectArea(POINT pos) abstract;

		//이벤트 우선순위를 결정해주는 함수
		//this가 우선순위이면 음수, 동일 우선순위이면 0, 
		//object가 우선순위이면 양수를 반환
		virtual BOOL event_priority(EventAcceptable& object) abstract;
	};
}


#endif