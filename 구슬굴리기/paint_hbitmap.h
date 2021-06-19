﻿#ifndef __paint_hbitmap_h__
#define __paint_hbitmap_h__

#include <Windows.h>
#include "bitmap.h"
#include "paint_hdc.h"

namespace RollingBall
{
	class Paint_hBitmap
	{
	private:
		static Bitmap m_bmp;
	public:
		class _windowBuffer
		{
		private:
		//public:
			HBITMAP m_windowBuffer;
			class _old
			{
			public:
				HBITMAP m_windowBuffer;

				_old() { init(); }
				BOOL isBackedUp();
				void init();
			} old;

			void backup(Paint_hDC& hDC);
			void rollback(Paint_hDC& hDC);

		public:
			_windowBuffer() { init(); }
			BOOL isSet();
			void init();

			//set 수행후 backup함수 실행하기 (완료)
			void set(const HWND& hwnd, Paint_hDC& hDC);

			//release 수행전 rollback 함수 실행하기 (완료)
			void release(Paint_hDC& hDC);

		} windowBuffer;

		class _res
		{
		private:
			vector<HBITMAP> m_res;
			BOOL flag_isSet;

			class _old
			{
			public:
				vector<HBITMAP> m_res;
				BOOL flag_isBackedUp;

			public:
				_old() {
					m_res = vector<HBITMAP>();
					init();
				}
				BOOL isBackedUp();
				void init();
				void resize(const size_t& newSize);
			} old;

			void backup(Paint_hDC& hDC);
			void rollback(Paint_hDC& hDC);

		public:
			_res() {
				m_res = vector<HBITMAP>();
				init();
			}
			BOOL isSet();
			void init();

			//set 수행후 backup함수 실행하기 (완료)
			void set(Paint_hDC& hDC);

			//release 수행전 rollback 함수 실행하기 (완료)
			void release(Paint_hDC& hDC);

			void resize(const size_t& newSize);
		} res;

	public:
		//클래스 변수 사용전 반드시 호출
		BOOL init(HINSTANCE hInstance);
		BOOL isInit();
		int bmpidx(Object& object, Scaler& scale, BOOL mask_texture = FALSE);
		int res_count();
		void resize_res_vector(const size_t& newSize);
	};
}

#endif