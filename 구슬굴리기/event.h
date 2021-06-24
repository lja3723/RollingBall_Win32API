#ifndef __event_h__
#define __event_h__

#include <vector>
#include <Windows.h>

using std::vector;

namespace RollingBall
{
	/*
	* WM 메세지를 편의성이 높도록 가공한 Event 객체 클래스
	*/
	class Event
	{
		//EventProducer가 Event를 초기화할 것임
		friend class EventProducer;

	protected:
		//이벤트의 유효성 저장
		BOOL m_isValid;
		//캡슐화된 진짜 winMsg 정보
		class _m_winMsg
		{
		public:
			UINT iMsg;
			WPARAM wParam;
			LPARAM lParam;

			_m_winMsg(UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0) {
				this->iMsg = iMsg;
				this->wParam = wParam;
				this->lParam = lParam;
			}
		} m_winMsg;

	public:
		///////////////////
		//	inner class
		// ////////////////
		//event의 winMsg 요소가 전달된 파라미터와 일치하는지 알려줌
		class _isWinMsg
		{
		private:
			Event* e;
		public:
			_isWinMsg(Event* e) { this->e = e; }
			BOOL iMsg(UINT iMsg) { return e->m_winMsg.iMsg == iMsg; }
			BOOL wParam(WPARAM wParam) { return e->m_winMsg.wParam == wParam; }
			BOOL lParam(LPARAM lParam) { return e->m_winMsg.lParam == lParam; }
		} isWinMsg;

		//event가 어떤 요소로 발생했는지 표현
		class _eventType
		{
		private:
			Event* e;
		public:
			_eventType(Event* e) { this->e = e; }
			BOOL isEquals(UINT iMsg) { return e->isWinMsg.iMsg(iMsg); }
		} eventType;

	public:
		/////////////////
		//	interface
		// //////////////
		//이벤트가 유효한지 확인
		BOOL isValid();

		//winMsg 요소로 이벤트 생성
		Event(UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0)
			: m_winMsg(iMsg, wParam, lParam), isWinMsg(this), eventType(this)
		{ 
			//winMsg 요소로 초기화할 경우 항상 유효함
			m_isValid = TRUE;
		}
		//복사 생성자
		Event(const Event& e)
			: Event(e.m_winMsg.iMsg, e.m_winMsg.wParam, e.m_winMsg.lParam) {
			m_isValid = e.m_isValid;
		}
	};

	class MouseEvent : public Event
	{
		//EventProducer가 MouseEvent를 초기화할 것임
		friend class EventProducer;
	protected:
		///////////////////
		//	inner class
		// ////////////////
		//마우스 이벤트 상태 저장
		class _state
		{
		public:
			static const int numofButtons	= 3;
			static const int LButton		= 0;
			static const int MButton		= 1;
			static const int RButton		= 2;

			static BOOL Buttons[numofButtons];
			static BOOL isInitButtonsArray;

			_state() { initButtonsArray(); }
			static void initButtonsArray();
		} state;

		//마우스 포인터 위치
		POINT pos;
		//마우스 스크롤 회전 방향
		int scroll;
		void init() {
			pos = { 0, 0 };
			scroll = 0;
		}

	public:
		//////////////////
		//	inner class
		//////////////////
		//event가 어떤 타입으로 발생했는지 표현
		class _eventType
		{
		private:
			MouseEvent* e;

		public:
			//eventType을 표현하는 상수 선언
			static const UINT LButtonDown			= WM_LBUTTONDOWN;
			static const UINT LButtonUp				= WM_LBUTTONUP;
			static const UINT LButtonDoubleClick	= WM_LBUTTONDBLCLK;

			static const UINT MButtonDown			= WM_MBUTTONDOWN;
			static const UINT MButtonUp				= WM_MBUTTONUP;
			static const UINT MButtonDoubleClick	= WM_MBUTTONDBLCLK;

			static const UINT RButtonDown			= WM_RBUTTONDOWN;
			static const UINT RButtonUp				= WM_RBUTTONUP;
			static const UINT RButtonDoubleClick	= WM_RBUTTONDBLCLK;

			static const UINT MouseMove				= WM_MOUSEMOVE;
			static const UINT MouseWheel			= WM_MOUSEWHEEL;

		public:
			//해당 event의 eventType이 무엇인지 알려주는 함수 선언
			_eventType(MouseEvent* e) { this->e = e; }

			BOOL isLButtonDown()		{ return e->isWinMsg.iMsg(LButtonDown); }
			BOOL isLButtonUp()			{ return e->isWinMsg.iMsg(LButtonUp); }
			BOOL isLButtonDoubleClick()	{ return e->isWinMsg.iMsg(LButtonDoubleClick); }

			BOOL isMButtonDown()		{ return e->isWinMsg.iMsg(MButtonDown); }
			BOOL isMButtonUp()			{ return e->isWinMsg.iMsg(MButtonUp); }
			BOOL isMButtonDoubleClick()	{ return e->isWinMsg.iMsg(MButtonDoubleClick); }

			BOOL isRButtonDown()		{ return e->isWinMsg.iMsg(RButtonDown); }
			BOOL isRButtonUp()			{ return e->isWinMsg.iMsg(RButtonUp); }
			BOOL isRButtonDoubleClick()	{ return e->isWinMsg.iMsg(RButtonDoubleClick); }

			BOOL isMouseMove()			{ return e->isWinMsg.iMsg(MouseMove); }
			BOOL isMouseWheel()			{ return e->isWinMsg.iMsg(MouseWheel); }
		} eventType;

	public:
		/////////////////
		//	interface
		// //////////////
		//winMsg 요소로 이벤트 생성
		MouseEvent(UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0)
			: Event(iMsg, wParam, lParam), eventType(this) { init(); }
		//Event로의 형변환 생성자
		MouseEvent(const Event& e)
			: Event(e), eventType(this) { init(); }
		//복사 생성자
		MouseEvent(const MouseEvent& e)
			: MouseEvent(e.m_winMsg.iMsg, e.m_winMsg.wParam, e.m_winMsg.lParam) {
			this->m_isValid = e.m_isValid;
		}
	};

	class KeyboardEvent : public Event
	{
		//EventProducer가 KeyboardEvent를 초기화할 것임
		friend class EventProducer;
	protected:
		//////////////////
		//	inner class
		//////////////////
		//키보드 이벤트 상태 저장
		class _state
		{
		public:
			//키보드 배열(키보드의 눌림여부 저장)
			static const int numofKeys = 256;
			static BOOL keys[numofKeys];
			//키보드 배열 초기화 수행
			static void initKeysArray();
			//키보드 배열 초기화 여부 저장
			static BOOL isInitKeysArray;
			WPARAM changedKey;
			_state() { initKeysArray(); }
		} state;

	protected:
		//////////////////
		//	functions
		//////////////////
		void key_down(WPARAM VK_msg);
		void key_up(WPARAM VK_msg);

	public:
		//////////////////
		//	inner class
		//////////////////
		//event가 어떤 요소로 발생했는지 표현함
		class _eventType
		{
		private:
			KeyboardEvent* e;

		//eventType을 표현하는 상수 선언
		public:
			static const UINT KeyDown = WM_KEYDOWN;
			static const UINT KeyUp = WM_KEYUP;
		
		//해당 event의 eventType이 무엇인지 알려주는 함수 선언
		public:
			_eventType(KeyboardEvent* e) { this->e = e; }
			BOOL isKeyDown() { return e->isWinMsg.iMsg(KeyDown); }
			BOOL isKeyUp() { return e->isWinMsg.iMsg(KeyUp); }
		} eventType;

	public:
		///////////////////
		//	interface
		///////////////////
		//winMsg 요소로 이벤트 생성
		KeyboardEvent(UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0)
			: Event(iMsg, wParam, lParam), eventType(this) {}
		//Event로의 형변환 생성자
		KeyboardEvent(const Event& e)
			: Event(e), eventType(this) {}
		//복사 생성자
		KeyboardEvent(const KeyboardEvent& e)
			: KeyboardEvent(e.m_winMsg.iMsg, e.m_winMsg.wParam, e.m_winMsg.lParam) {
			this->m_isValid = e.m_isValid;
			state.changedKey = e.state.changedKey;
		}


		//키보드 눌린 상태 반환
		BOOL isKeyPressed(WPARAM VK_msg);
		//상태가 변화한 키보드 키 반환
		BOOL isKeyStateChanged(WPARAM VK_msg);
		//키를 눌렀는지 반환
		BOOL isKeyDown(WPARAM VK_msg);
		//키를 뗐는 지 반환
		BOOL isKeyUp(WPARAM VK_msg);
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
	* 이벤트를 처리하기 위해서는 가상 함수들을 구현해야 함
	*/
	class EventAcceptable
	{
		friend class EventProducer;
	private:
		//EventAcceptable을 상속한 클래스의 인스턴스 주소값 벡터
		static vector<EventAcceptable*> object_ref;

	protected:
		//마우스 이벤트를 받아들이려면 오버라이딩할 것
		virtual void event_mouse(MouseEvent e) {}

		//키보드 이벤트를 받아들이려면 오버라이딩할 것
		virtual void event_keyboard(KeyboardEvent e) {}

		//기타 이벤트를 받아들이려면 오버라이딩할 것
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