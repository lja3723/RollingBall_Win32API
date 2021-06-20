#ifndef __paint_hbitmap_h__
#define __paint_hbitmap_h__

#include <Windows.h>
#include "bitmap.h"
//#include "paint_hdc.h"

namespace RollingBall
{
	class Paint_hBitmap
	{
	private:
		static Bitmap m_bmp;

	public:
		//////////////////
		//	inner class
		//////////////////
		class _windowBuffer
		{
		private:
			HBITMAP m_windowBuffer;

		public:
			////////////////////////
			//	windowBuffer.old
			////////////////////////
			class _old
			{
			private:
				HBITMAP m_windowBuffer;

			public:
				_old() { m_windowBuffer = NULL; }
				operator HBITMAP ();
				void operator=(HBITMAP hBitmap);
			} old;

		public:
			////////////////////////////////
			//	windowBuffer interface
			////////////////////////////////
			_windowBuffer() { init(); }
			BOOL isSet();
			BOOL isBackedUp();
			void init();
			//windowRect크기의 화면 DC 호환 windowBuffer를 생성
			void set(RECT& windowRect, const HDC& window);
			void release();
			operator HBITMAP ();

		} windowBuffer;
		class _res
		{
		private:
			vector<HBITMAP> m_res;
			BOOL flag_isSet;
			BOOL flag_isBackedUp;

		public:
			////////////////
			//	res.old
			////////////////
			class _old
			{
			private:
				vector<HBITMAP> m_res;

			public:
				_old() {
					m_res = vector<HBITMAP>();
					init();
				}
				HBITMAP& operator()(int idx);
				void init();
				void resize(const size_t& newSize);
			} old;

		public:
			////////////////////
			//	res interface
			////////////////////
			_res() {
				m_res = vector<HBITMAP>();
				init();
			}
			BOOL isSet();
			BOOL isBackedUp();
			void isBackedUp(BOOL flag);
			void init();
			void set();
			void release();
			void resize(const size_t& newSize);
			size_t size();
			const HBITMAP& operator()(int idx);
		} res;

	public:
		//////////////////////////
		//	interface
		//////////////////////////
		BOOL isInit();
		//클래스 변수 사용전 반드시 호출
		BOOL init(HINSTANCE hInstance);
		int bmpidx(Object& object, Scaler& scale, BOOL mask_texture = FALSE);
		int res_count();
		void resize_res_vector(const size_t& newSize);
	};
}

#endif