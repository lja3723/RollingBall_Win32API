#ifndef __paint_hbitmap_h__
#define __paint_hbitmap_h__

#include <Windows.h>
#include "bitmap.h"
#include "paint_hdc.h"

namespace RollingBall
{
	class Paint_hBitmap
	{
	public:
		class _windowBuffer
		{
		private:
			HBITMAP m_windowBuffer;
		public:
			BOOL isSet();
			void init();
			void set(const HWND& hwnd, Bitmap& bmp, const HDC& hDC_window);
			void release(Bitmap& bmp);

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
			void set(Bitmap& bmp);
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
	};
}

#endif