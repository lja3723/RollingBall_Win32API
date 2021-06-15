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
		public:
			BOOL isSet();
			void init();
			void set(const HWND& hwnd, const HDC& hDC_window);
			void release();

			class _old
			{
			private:
				HBITMAP m_windowBuffer;
			public:
				BOOL isBackedUp();
				void init();
				void backup(Paint_hDC& hDC);
				void rollback(Paint_hDC& hDC);
			} old;

		} windowBuffer;

		class _res
		{
		private:
			vector<HBITMAP> m_res;
			BOOL flag_isSet;
		public:
			BOOL isSet();
			void init();
			void set();
			void resize(const size_t& newSize);

			class _old
			{
			private:
				vector<HBITMAP> m_res;
				BOOL flag_isBackedUp;
			public:
				BOOL isBackedUp();
				void init();
				void backup(Paint_hDC& hDC);
				void rollback(Paint_hDC& hDC);
				void resize(const size_t& newSize);
			} old;
		} res;

	public:
		BOOL init(HINSTANCE hInstance);
		BOOL isInit();
		int bmpidx(Object& object, Scaler& scale, BOOL mask_texture = FALSE);
		int res_count();
	};
}

#endif