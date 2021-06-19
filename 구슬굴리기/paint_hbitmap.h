#ifndef __paint_hbitmap_h__
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
			HBITMAP m_windowBuffer;

			class _old
			{
			public:
				HBITMAP m_windowBuffer;

				_old() { init(); }
				BOOL isBackedUp();
				void init();
			} old;

		public:
			//아래 두 함수는 hDC로 옮기기
			void backup(Paint_hDC& hDC);
			void rollback(Paint_hDC& hDC);

		public:
			_windowBuffer() { init(); }
			BOOL isSet();
			void init();
			//windowRect크기의 화면 DC 호환 windowBuffer를 생성
			//backup, rollback과의 연관성 제거
			//void set(RECT& windowRect, Paint_hDC& hDC);
			//void release(Paint_hDC& hDC);
			void set(RECT& windowRect, const HDC& window);
			void release();

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

		public:
			//아래 두 함수는 hDC로 옮기기
			void backup(Paint_hDC& hDC);
			void rollback(Paint_hDC& hDC);

		public:
			_res() {
				m_res = vector<HBITMAP>();
				init();
			}
			BOOL isSet();
			void init();
			//backup, rollback과의 연관성 제거
			//void set(Paint_hDC& hDC);
			//void release(Paint_hDC& hDC);
			void set();
			void release();
			void resize(const size_t& newSize);
		} res;

	public:
		//클래스 변수 사용전 반드시 호출
		BOOL init(HINSTANCE hInstance);
		BOOL isInit();
		int bmpidx(Object& object, Scaler& scale, BOOL mask_texture = FALSE);
		int res_count();
		void resize_res_vector(const size_t& newSize);
		//backup, rollback과의 연관성 제거
		//void set(RECT& windowRect, Paint_hDC& hDC);
		//void release(Paint_hDC& hDC);
		//void set(RECT& windowRect, const HDC& window);
		//void release();
	};
}

#endif