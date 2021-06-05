#ifndef __paint_hdc_h__
#define __paint_hdc_h__
#include <Windows.h>
#include <vector>

using std::vector;

namespace RollingBall
{
	class Paint_hDC
	{
		//friend class Paint;
	private:
		HWND hwnd;
		PAINTSTRUCT ps;
		int res_count;

		HDC m_window;

		//old code
		/*
		struct _mem {
			HDC windowBuffer;
			vector<HDC> res;
		} m_mem;
		*/
		//old code end


	public:
		/********************************
		*
		*	bool returns
		*
		*********************************/
		//hDC.window를 얻는 모드가 무엇인지 알려줌
		class _isWindowMode
		{
			friend class Paint_hDC;
		private:
			BOOL flag_GetDC;
		public:
			_isWindowMode() { flag_GetDC = FALSE; }
			BOOL GetDC();
			BOOL BeginPaint();
		} isWindowMode;

		//변수가 세팅되었는지 알려줌
		class _isSet
		{
			friend class Paint_hDC;
		private:
			Paint_hDC* hDC;
			BOOL flag_MemDCres;
		public:
			_isSet(Paint_hDC* m_hDC) { hDC = m_hDC; flag_MemDCres = FALSE; }
			BOOL window();
			BOOL memWindowBuffer();
			BOOL memRes();
		} isSet;


		/********************************
		*
		*	hDC.window management
		*
		*********************************/
		//hDC.window를 어떤 방식으로 얻을지 설정함
		//BeginPaint 또는 GetDC으로 얻을 수 있음
		class _windowMode
		{
			friend class Paint_hDC;
		private:
			Paint_hDC* hDC;
		public:
			_windowMode(Paint_hDC* m_hDC) {	hDC = m_hDC; }
			void set_BeginPaint();
			void set_GetDC();
		}windowMode;

		//hDC.window를 관리함
		class _window
		{
			friend class Paint_hDC;
		private:
			Paint_hDC* hDC;
		public:
			_window(Paint_hDC* m_hDC) { hDC = m_hDC; }
			void init();
			void set();
			HDC& get();
			void release();
		}window;



		/********************************
		*
		*	hDC.mem management
		*
		*********************************/
		//hDC.mem 요소를 관리함
		class _mem
		{
			friend class Paint_hDC;
		private:
			struct _m_mem {
				HDC windowBuffer;
				vector<HDC> res;
			} m_mem;

			Paint_hDC* hDC;
		public:
			_mem(Paint_hDC* m_hDC) : windowBuffer(this, m_hDC), res(this, m_hDC)
			{ hDC = m_hDC; }

			class _windowBuffer
			{
				friend class _mem;
			private:
				Paint_hDC* hDC;
				_mem* memDC;
			public:
				//_windowBuffer(Paint_hDC* m_hDC) { hDC = m_hDC; }
				_windowBuffer(_mem* m_memDC, Paint_hDC* m_hDC) 
				{ memDC = m_memDC; hDC = m_hDC; }

				void init();
				void set();
				HDC& get();
				void release();

			} windowBuffer;

			class _res
			{
				friend class _mem;
			private:
				Paint_hDC* hDC;
				_mem* memDC;
			public:
				//_res(Paint_hDC* m_hDC) { hDC = m_hDC; }
				_res(_mem* m_memDC, Paint_hDC* m_hDC) { memDC = m_memDC; hDC = m_hDC;	}

				void init();
				void set();
				vector<HDC>& get();
				void release();
				void resize(size_t newSize);
			} res;

			void create();
			void del();
		} mem;



		Paint_hDC(HWND m_hwnd = NULL, int m_res_count = 0)
			: hwnd(m_hwnd), res_count(m_res_count), 
			isSet(this), windowMode(this), window(this), mem(this)
		{
			m_window = NULL;
		};

		void init(HWND m_hwnd, int m_res_count);
	};
}

#endif