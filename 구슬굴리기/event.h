#ifndef __event_h__
#define __event_h__

#include <vector>
#include <Windows.h>

using std::vector;

namespace RollingBall
{
	//윈도우 메세지 저장 클래스
	class WindowMessage
	{
	public:
		HWND hwnd;
		UINT iMsg;
		WPARAM wParam;
		LPARAM lParam;

		WindowMessage(HWND hwnd = NULL, UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0)
		{
			this->hwnd = hwnd;
			this->iMsg = iMsg;
			this->wParam = wParam;
			this->lParam = lParam;
		}
	};



	/*
	* WM 메세지를 편의성이 높도록 가공한 Event 객체 클래스
	*/
	class Event
	{
	protected:
		BOOL m_isValid;
		WindowMessage m_winMsg;
		void init();

	public:
		///////////////////
		//	inner class
		// ////////////////
		//캡슐화된 winMsg를 read only로 리턴하는 인터페이스
		class _winMsg
		{
		private:
			WindowMessage* wm;
		public:
			_winMsg(WindowMessage* wm)		 { this->wm = wm; }
			const UINT iMsg()				 { return wm->iMsg; }
			const WPARAM wParam()			 { return wm->wParam; }
			const LPARAM lParam()			 { return wm->lParam; }
			BOOL iMsgEquals(UINT iMsg)		 { return this->iMsg()	 == iMsg; }
			BOOL wParamEquals(WPARAM wParam) { return this->wParam() == wParam; }
			BOOL lParamEquals(LPARAM lParam) { return this->lParam() == lParam; }
		} winMsg;

	public:
		/////////////////
		//	interface
		// //////////////
		//이벤트가 유효한지 확인
		BOOL isValid();

		//winMsg 요소로 이벤트 생성
		Event(HWND hwnd = NULL, UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0) : winMsg(&m_winMsg) 
			{ m_winMsg = WindowMessage(hwnd, iMsg, wParam, lParam); init(); }
		Event(WindowMessage wm) : Event(wm.hwnd, wm.iMsg, wm.wParam, wm.lParam) {}
	};

	class MouseEvent : public Event
	{
	protected:
		///////////////////
		//	inner class
		// ////////////////
		//마우스 이벤트 상태 저장
		class _staticState
		{
		public:
			static const int numofButtons	= 3;
			static const int LButton		= 0;
			static const int MButton		= 1;
			static const int RButton		= 2;

			static BOOL buttons[numofButtons];
			static BOOL isInitButtonsArray;

		public:
			_staticState() { initButtonsArray(); }
			static void initButtonsArray();
			void button_down(int button_idx);
			void button_up(int button_idx);
		} staticState;
		class _localState
		{
		public:
			//마우스 포인터 위치
			POINT pos;
			//마우스 스크롤 회전 방향
			int scroll;

			_localState() {
				pos = { 0, 0 };
				scroll = 0;
			}
		} localState;
		void init();

	public:
		//////////////////
		//	inner class
		//////////////////
		//event가 어떤 타입으로 발생했는지 표현
		class _eventType
		{
		private:
			_winMsg* wm;
			_localState* st;

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
			_eventType(_winMsg* wm, _localState* st) { this->wm = wm, this->st = st; }

			BOOL isLButtonDown()		{ return wm->iMsgEquals(LButtonDown); }
			BOOL isLButtonUp()			{ return wm->iMsgEquals(LButtonUp); }
			BOOL isLButtonDoubleClick()	{ return wm->iMsgEquals(LButtonDoubleClick); }

			BOOL isMButtonDown()		{ return wm->iMsgEquals(MButtonDown); }
			BOOL isMButtonUp()			{ return wm->iMsgEquals(MButtonUp); }
			BOOL isMButtonDoubleClick()	{ return wm->iMsgEquals(MButtonDoubleClick); }

			BOOL isRButtonDown()		{ return wm->iMsgEquals(RButtonDown); }
			BOOL isRButtonUp()			{ return wm->iMsgEquals(RButtonUp); }
			BOOL isRButtonDoubleClick()	{ return wm->iMsgEquals(RButtonDoubleClick); }

			BOOL isMouseMove()			{ return wm->iMsgEquals(MouseMove); }
			BOOL isMouseWheel()			{ return wm->iMsgEquals(MouseWheel); }
			BOOL isMouseWheelUp()		{ return isMouseWheel() && st->scroll > 0; }
			BOOL isMouseWheelDown()		{ return isMouseWheel() && st->scroll < 0; }
		} eventType;

	public:
		/////////////////
		//	interface
		// //////////////
		//winMsg 요소로 이벤트 생성
		MouseEvent(HWND hwnd = NULL, UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0)
			: Event(hwnd, iMsg, wParam, lParam), eventType(&winMsg, &localState) { init(); }
		MouseEvent(WindowMessage wm) : MouseEvent(wm.hwnd, wm.iMsg, wm.wParam, wm.lParam) {} 

		POINT pos();
		BOOL isLButtonDown();
		BOOL isMButtonDown();
		BOOL isRButtonDown();
	};

	class KeyboardEvent : public Event
	{
	protected:
		//////////////////
		//	inner class
		//////////////////
		//키보드 이벤트 상태 저장
		class _staticState
		{
		public:
			//키보드 배열(키보드의 눌림여부 저장)
			static const int numofKeys = 256;
			static BOOL keys[numofKeys];
			//키보드 배열 초기화 수행
			static void initKeysArray();
			//키보드 배열 초기화 여부 저장
			static BOOL isInitKeysArray;
			_staticState() { initKeysArray(); }
		} staticState;
		class _localState
		{
		public:
			WPARAM changedKey;
			_localState() { changedKey = 0; }
		} localState;

	protected:
		//////////////////
		//	functions
		//////////////////
		void key_down(WPARAM VK_msg);
		void key_up(WPARAM VK_msg);
		void init();

	public:
		//////////////////
		//	inner class
		//////////////////
		//event가 어떤 요소로 발생했는지 표현함
		class _eventType
		{
		private:
			_winMsg* wm;

		//eventType을 표현하는 상수 선언
		public:
			static const UINT KeyDown = WM_KEYDOWN;
			static const UINT KeyUp = WM_KEYUP;
		
		//해당 event의 eventType이 무엇인지 알려주는 함수 선언
		public:
			_eventType(_winMsg* wm) { this->wm = wm; }
			BOOL isKeyDown() { return wm->iMsgEquals(KeyDown); }
			BOOL isKeyUp() { return wm->iMsgEquals(KeyUp); }
		} eventType;

	public:
		///////////////////
		//	interface
		///////////////////
		//winMsg 요소로 이벤트 생성
		KeyboardEvent(HWND hwnd = NULL, UINT iMsg = 0, WPARAM wParam = 0, LPARAM lParam = 0)
			: Event(hwnd, iMsg, wParam, lParam), eventType(&winMsg) { init(); }
		KeyboardEvent(WindowMessage wm) : KeyboardEvent(wm.hwnd, wm.iMsg, wm.wParam, wm.lParam) {}

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
	class EventSender
	{
	public:
		static void translate_windowEvent(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	};



	/*
	* 이벤트를 처리할 수 있는 능력을 부여하는 인터페이스 클래스
	* 이벤트를 처리하기 위해서는 가상 함수들을 구현해야 함
	*/
	class EventAcceptable
	{
		friend class EventSender;
	private:
		//EventAcceptable을 상속한 클래스의 인스턴스 주소값 벡터
		static vector<EventAcceptable*> object_ref;

	protected:
		//마우스 이벤트를 받아들이려면 오버라이딩할 것
		virtual void event_mouse(MouseEvent e) {}

		//키보드 이벤트를 받아들이려면 오버라이딩할 것
		virtual void event_keyboard(KeyboardEvent e) {}

		//기타 이벤트를 받아들이려면 오버라이딩할 것
		virtual void event_else(Event e) {}
	public:
		EventAcceptable();
		~EventAcceptable();
	};
}


#endif