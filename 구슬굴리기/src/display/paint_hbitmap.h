#ifndef __paint_hbitmap_h__
#define __paint_hbitmap_h__

#include <Windows.h>
#include <vector>
#include "bitmap.h"
#include "paint_hdc.h"

using std::vector;

namespace RollingBall
{

	class Paint_hBitmap
	{
	private:
		BOOL flag_isInit;
	public:
		class _windowBuffer
		{
		private:
			HBITMAP m_windowBuffer;
			HWND m_hwnd;
			Bitmap* m_bmp;
			Paint_hBitmap* m_hBitmap;
			Paint_hDC* m_hDC;
			RECT windowRect;
		public:
			class _old
			{
			private:
				Paint_hDC* m_hDC;
				Paint_hBitmap* m_hBitmap;
				HBITMAP m_windowBuffer;
			public:

				BOOL isBackedUp();
				void backup();
				void rollback();

				_old() { 
					m_windowBuffer = NULL; 
					m_hDC = NULL;
					m_hBitmap = NULL;
				}

				void init(Paint_hDC* hDC, Paint_hBitmap* hBitmap);
			} old;

			BOOL isSet();
			void set();
			void release();

			const HBITMAP& operator()();

			_windowBuffer() { 
				m_windowBuffer = NULL;
				m_hwnd = NULL;
				m_bmp = NULL;
				m_hBitmap = NULL;
				m_hDC = NULL;
				memset(&windowRect, 0, sizeof(windowRect));
			}

			void init(HWND hwnd, Bitmap* bmp, Paint_hDC* hDC, Paint_hBitmap* hBitmap);
		} windowBuffer;
		class _res
		{
		private:
			vector<HBITMAP> m_res;
			BOOL flag_isSet;
			Paint_hDC* m_hDC;
			Bitmap* m_bmp;
		public:
			class _old
			{
			private:
				vector<HBITMAP> m_res;
				BOOL flag_isBackedUp;
				Paint_hDC* m_hDC;
				_res* m_res_outer;
			public:

				BOOL isBackedUp();
				void backup();
				void rollback();
				void resize(size_t newSize);
				_old() { 
					flag_isBackedUp = FALSE;
					m_hDC = NULL;
					m_res_outer = NULL;
				}

				void init(_res* res_outer, Paint_hDC* hDC);
			} old;

			BOOL isSet();
			void set();
			void resize(size_t newSize);

			_res() { 
				flag_isSet = FALSE; 
				m_hDC = NULL;
				m_bmp = NULL;
			}

			void init(Paint_hDC* hDC, Bitmap* bmp);
		} res;

		Paint_hBitmap() { flag_isInit = FALSE; }

		void init(HWND hwnd, Bitmap* bmp, Paint_hDC* hDC);
		BOOL isInit();
	};
}


#endif