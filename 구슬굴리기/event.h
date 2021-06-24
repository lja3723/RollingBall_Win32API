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
		//EventProducer가 Event를 초기화할 것임
		friend class EventProducer;

	protected:
		//키보드 배열(키보드의 눌림여부 저장)
		static const int numofKeys = 256;
		static BOOL keys[numofKeys];
		//키보드 배열 초기화 여부 저장
		static BOOL isInitKeysArray;
		//키보드 배열 초기화 수행
		void initKeysArray();
		//이벤트의 유효성 저장
		BOOL m_isValid;
		//캡슐화된 진짜 winMsg 정보
		class _m_winMsg
		{
		public:
			UINT iMsg;
			WPARAM wParam;
			LPARAM lParam;

			_m_winMsg(UINT m_iMsg = 0, WPARAM m_wParam = 0, LPARAM m_lParam = 0) {
				iMsg = m_iMsg;
				wParam = m_wParam;
				lParam = m_lParam;
			}
			UINT getiMsg() { return iMsg; }
			WPARAM getwParam() { return wParam; }
			LPARAM getlParam() { return lParam; }
		} m_winMsg;

	public:
		//winMsg 요소를 read only할 수 있는 public interface
		class _winMsg
		{
		private:
			_m_winMsg* winMsg;
		public:
			_winMsg(_m_winMsg* m_winMsg) { winMsg = m_winMsg; }
			const UINT iMsg() { return winMsg->getiMsg(); }
			const WPARAM wParam() { return winMsg->getwParam(); }
			const LPARAM lParam() { return winMsg->getlParam(); }
		} winMsg;

		//이벤트가 유효한지 확인
		BOOL isValid();

		//winMsg 요소로 이벤트 생성
		Event(UINT m_iMsg = 0, WPARAM m_wParam = 0, LPARAM m_lParam = 0)
			: m_winMsg(m_iMsg, m_wParam, m_lParam), winMsg(&m_winMsg)
		{ 
			initKeysArray();
			//winMsg 요소로 초기화할 경우 항상 유효함
			m_isValid = TRUE;
		}
		//복사 생성자
		Event(const Event& e)
			: Event(e.m_winMsg.iMsg, e.m_winMsg.wParam, e.m_winMsg.lParam) {
			m_isValid = e.m_isValid;
		}
	};

	/*
	* 마우스 이벤트가 발생하면 그와 관련된 정보를 전달할 이벤트 객체
	*/
	class MouseEvent : public Event
	{
		//EventProducer가 MouseEvent를 초기화할 것임
		friend class EventProducer;
	protected:
		void init() {
			pos = { 0, 0 };
			scroll = 0;
		}
		//마우스 포인터 위치
		POINT pos;
		//마우스 스크롤 회전 방향
		int scroll;
	public:
		//winMsg 요소로 이벤트 생성
		MouseEvent(UINT m_iMsg = 0, WPARAM m_wParam = 0, LPARAM m_lParam = 0)
			: Event(m_iMsg, m_wParam, m_lParam) { init(); }
		//Event로의 형변환 생성자
		MouseEvent(const Event& e)
			: Event(e) { init(); }
	};

	/*
	* 키보드 이벤트가 발생하면 그와 관련된 정보를 전달할 이벤트 객체
	*/
	class KeyboardEvent : public Event
	{
		//EventProducer가 KeyboardEvent를 초기화할 것임
		friend class EventProducer;
	protected:
		void key_down(WPARAM VK_msg);
		void key_up(WPARAM VK_msg);
	public:
		//winMsg 요소로 이벤트 생성
		KeyboardEvent(UINT m_iMsg = 0, WPARAM m_wParam = 0, LPARAM m_lParam = 0)
			: Event(m_iMsg, m_wParam, m_lParam) {}
		//Event로의 형변환 생성자
		KeyboardEvent(const Event& e)
			: Event(e) {}

		//키보드 눌린 상태 반환
		BOOL isKeyDown(WPARAM VK_msg);
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