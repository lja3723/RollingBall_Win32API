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
		private:
			void init();

		public:
			_window() { init(); }
			void set(HWND hwnd);
			operator const HDC&();
			void release(const HWND& hwnd);
			BOOL isSet();

		}window;

		//hDC.mem 요소를 관리함
		class _mem
		{
		public:
			class _windowBuffer
			{
			private:
				//Paint_hDC* m_hDC;
				HDC m_windowBuffer;

			private:
				void init();

			public:
				_windowBuffer()
				{ init(); }

				void set(const HDC& window);
				operator const HDC&();				
				void release();
				BOOL isSet();

			} windowBuffer;
			class _res
			{
			private:
				vector<HDC> m_res;
				BOOL flag_isSet;

			private:
				void init();

			public:
				_res() { init(); }

				void set(const HDC& mem_windowBuffer);
				const HDC& operator[](int idx);
				void release();
				void resize(const size_t& newSize);
				BOOL isSet();
			} res;

		public:
			void set(const HDC& window, const HDC& mem_windowBuffer);
			void release();
		} mem;

		void resize_res_vector(const size_t& newSize);
	};
}

#endif