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
	class Event
	{
	//키보드 눌림 저장
	protected:
		static BOOL isInit;
		static const int numofKeys = 256;
		static BOOL keys[numofKeys];
		void init();

	public:
		BOOL isValid;
		class _winmsg
		{
		public:
			UINT iMsg;
			WPARAM wParam;
			LPARAM lParam;
		} winmsg;
		//Event() = delete;
		Event(UINT m_iMsg = 0, WPARAM m_wParam = 0, LPARAM m_lParam = 0, BOOL m_isValid = TRUE)
		{ 
			init();
			isValid = m_isValid;
			winmsg.iMsg = m_iMsg;
			winmsg.wParam = m_wParam;
			winmsg.lParam = m_lParam;
		}
		Event(const Event& e)
			: Event(e.winmsg.iMsg, e.winmsg.wParam, e.winmsg.lParam, e.isValid) {}

		//키보드 눌린 상태 반환
		BOOL isKeyDown(WPARAM VK_msg);
	};

	/*
	* 마우스 이벤트가 발생하면 그와 관련된 정보를 전달할 이벤트 객체
	*/
	class MouseEvent : public Event
	{
	private:
		void init() {
			pos = { 0, 0 };
			scroll = 0;
		}
	public:
		POINT pos;
		int scroll;
		MouseEvent(UINT m_iMsg = 0, WPARAM m_wParam = 0, LPARAM m_lParam = 0, BOOL m_isValid = TRUE)
			: Event(m_iMsg, m_wParam, m_lParam, m_isValid) { init(); }
		MouseEvent(const Event& e)
			: Event(e) { init(); }
	};

	/*
	* 키보드 이벤트가 발생하면 그와 관련된 정보를 전달할 이벤트 객체
	*/
	class KeyboardEvent : public Event
	{
		friend class EventProducer;
	private:
		void key_down(WPARAM VK_msg);
		void key_up(WPARAM VK_msg);
	public:
		KeyboardEvent(UINT m_iMsg = 0, WPARAM m_wParam = 0, LPARAM m_lParam = 0)
			: Event(m_iMsg, m_wParam, m_lParam) {}
		KeyboardEvent(const Event& e)
			: Event(e) {}
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
		static Event produce_Event(UINT iMsg, WPARAM wParam, LPARAM lParam);
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
		//마우스 이벤트를 받아들이기 위해 오버로딩 필요
		virtual void event_mouse(MouseEvent e) {}

		//키보드 이벤트를 받아들이기 위해 오버로딩 필요
		virtual void event_keyboard(KeyboardEvent e) {}

		//기타 이벤트를 받아들이기 위해 오버로딩 필요
		virtual void event_all(Event e) {}

		//virtual BOOL isObjectArea(POINT& pos) { return FALSE; }

		//이벤트 우선순위를 결정해주는 함수
		//this가 우선순위이면 음수, 동일 우선순위이면 0, 
		//object가 우선순위이면 양수를 반환
		//virtual BOOL event_priority(EventAcceptable& object) { return FALSE; }

	public:
		EventAcceptable();
		~EventAcceptable();
	};
}


#endif