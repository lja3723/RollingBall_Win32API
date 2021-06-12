#ifndef __paint_hdc_h__
#define __paint_hdc_h__
#include <Windows.h>
#include <vector>

using std::vector;

namespace RollingBall
{
	class Paint_hDC
	{
	//public class
	public:
		//hDC.window를 관리함
		class _window
		{
		private:
			HDC m_window;
			HWND m_hwnd;
			PAINTSTRUCT m_ps;

		public:
			//window를 어떤 방식으로 얻을지 설정함
			//BeginPaint 또는 GetDC으로 얻을 수 있음
			class _mode
			{
			private:
				BOOL flag_isGetDC;

			public:
				_mode() { flag_isGetDC = FALSE; }
				void set_BeginPaint();
				void set_GetDC();
				BOOL isGetDC();
				BOOL isBeginPaint();

			}mode;

		public:
			_window(HWND hwnd)
			{
				m_window = NULL;
				m_hwnd = hwnd;
				memset(&m_ps, 0, sizeof(m_ps));
			}
			void init(HWND hwnd);
			void set();
			HDC& operator()();
			void release();
			BOOL isSet();

		}window;

		//hDC.mem 요소를 관리함
		class _mem
		{
		public:
			class _windowBuffer
			{
			private:
				Paint_hDC* m_hDC;
				HDC m_windowBuffer;
			public:
				_windowBuffer(Paint_hDC * hDC)
				{ m_hDC = hDC; m_windowBuffer = NULL; }

				void init();
				void set();
				HDC& operator()();
				void release();
				BOOL isSet();

			} windowBuffer;
			class _res
			{
			private:
				Paint_hDC* m_hDC;
				vector<HDC> m_res;
				BOOL flag_isSet;
			public:
				_res(Paint_hDC* hDC) { m_hDC = hDC; flag_isSet = FALSE; }

				void init();
				void set();
				HDC operator()(int idx);
				void operator()(int idx, HDC m_hdc);
				void release();
				void resize(size_t newSize);
				BOOL isSet();
			} res;

		public:
			_mem(Paint_hDC* hDC) : windowBuffer(hDC), res(hDC) {	}
			void create();
			void del();
		} mem;

	//public function
	public:
		Paint_hDC(HWND hwnd = NULL) : window(hwnd), mem(this) {}
		void init(HWND hwnd);
	};
}

#endif