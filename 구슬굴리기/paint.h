#pragma once
#ifndef __paint_h__
#define __paint_h__

#include <Windows.h>
#include <vector>
#include "bitmap.h"
#include "object.h"
#include "scaler.h"

using std::vector;

/*
*
* Paint:
* 프로그램의 시각적 표현을 전담하는 클래스를 정의
*
*/

namespace RollingBall
{
	class Paint
	{
	private:
		Bitmap bmp;
		Scaler scale;

		//리소스 개수를 저장함
		static int res_count;

		//Win32API에서 사용되는 변수들의 집합체
		struct _winAPI{
			HINSTANCE hInstance;
			HWND hwnd;
			PAINTSTRUCT ps;
			RECT windowRect;

			struct hDC {
				HDC window;
				struct _mem {
					HDC windowBuffer;
					vector<HDC> res;
				} mem;
			} hDC;

			struct _hBitmap{
				HBITMAP windowBuffer;
				vector<HBITMAP> res;
				struct _old{
					HBITMAP windowBuffer;
					vector<HBITMAP> res;
				} old;
			} hBitmap;
		} winAPI;
		
		//여러가지 플래그를 저장하는 변수
		struct _flag{
			BOOL isHDCwindowMode_GetDC;

			BOOL isSetMemDCres;
			BOOL isSetHBitmapRes;
			BOOL isBackedUpHBitmapRes;

			BOOL isDoubleBufferingStart;
			BOOL isInitDoubleBuffering;
			BOOL isWindowSizeChanged;
		} flag;
	
	public:
		~Paint();

		//PrantManager 클래스 변수를 사용하기 전 반드시 수행해야 한다
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd);
		void translate_windowEvent(UINT m_iMsg, WPARAM m_wParam, LPARAM m_lParam);

		//scaler의 px_rate를 설정한다
		void scale_set(pixel px_rate);

		//페인트를 시작한다
		void begin();

		//페인트를 마친다
		void end();

		//배경을 페인트한다
		void background(Object& background);

		//오브젝트를 페인트한다
		void operator()(Object& obj);

		//오브젝트 정보를 페인트한다
		void info(Object& obj);
	
	
	private:
		/********************************
		*
		*	init management
		*
		*********************************/
		//클래스의 각종 플래그변수를 초기화
		//Paint::init()에서만 호출되어야 함
		void init_flags();
		void init_res_count();
		void init_res_vectors();



		/********************************
		*
		*	bool returns
		*
		*********************************/
		//hDC.window를 얻는 모드가 무엇인지 알려줌
		BOOL isHDCwindowMode_GetDC();
		BOOL isHDCwindowMode_BeginPaint();

		//변수가 세팅되었는지 알려줌
		BOOL isSetHDCwindow();
		BOOL isSetMemDCwindowBuffer();
		BOOL isSetMemDCres();
		BOOL isSetHBitmapWindowBuffer();
		BOOL isSetHBitmapRes();
		BOOL isBackedUpHBitmapWindowBuffer();
		BOOL isBackedUpHBitmapRes();

		//기타 정보를 알려줌
		BOOL isInit();
		BOOL isDoubleBufferingStart();
		BOOL isReadyToPaint();
		BOOL isInitDoubleBuffering();
		BOOL isWindowSizeChanged();


		/********************************
		*
		*	hDC.window management
		*
		*********************************/
		//hDC.window를 어떤 방식으로 얻을지 설정함
		//BeginPaint 또는 GetDC으로 얻을 수 있음
		void hDCwindowMode_set_BeginPaint();
		void hDCwindowMode_set_GetDC();
	
		//hDC.window를 관리함
		void hDCwindow_init();
		void hDCwindow_set();
		void hDCwindow_release();
		


		/********************************
		*
		*	hDC.mem management
		*
		*********************************/
		//hDC.mem 요소를 관리함
		void memDC_windowBuffer_init();
		void memDC_windowBuffer_set();
		void memDC_windowBuffer_release();

		void memDC_res_init();
		void memDC_res_set();
		void memDC_res_release();

		void memDC_create();
		void memDC_delete();



		/********************************
		*
		*	hBitmap management
		*
		*********************************/
		//hBitmap 변수를 관리함
		void hBitmap_windowBuffer_init();
		void hBitmap_windowBuffer_set();
		void hBitmap_windowBuffer_release();
		void hBitmap_res_init();
		void hBitmap_res_set();

		//hBitmap.old 변수를 관리함
		void hBitmap_old_windowBuffer_init();
		void hBitmap_old_windowBuffer_backup();
		void hBitmap_old_windowBuffer_rollback();
		void hBitmap_old_res_init();
		void hBitmap_old_res_backup();
		void hBitmap_old_res_rollback();



		/********************************
		*
		*	double buffering management
		*
		*********************************/
		//더블버퍼링을 처음으로 시작하기 위한 준비를 함
		void doubleBuffering_init();
		//더블버퍼링을 시작하고 종료함
		void doubleBuffering_start();
		void doubleBuffering_stop();
		//프로그램을 종료할 때 진행함
		void doubleBuffering_halt();




		/********************************
		*
		*	paint management
		*
		*********************************/
		//오브젝트를 버퍼에 그림
		void paint_background_tobuffer(Object& background);
		void paint_background_ruller_tobuffer();
		//void paint_ball_tobuffer(int x, int y, int ballsize = 64);
		void paint_tobuffer(Object& object);

		void paint_info_tobuffer(Object& object);

		//버퍼에 그려진 그림을 윈도우로 출력
		void flush_buffer();




		/********************************
		*
		*	etc
		*
		*********************************/
	};
}

#endif