#pragma once
#ifndef __PaintManager_h__
#define __PaintManager_h__

#include <Windows.h>
#include "BitmapManager.h"

/*
*
* PaintManager:
* 프로그램의 시각적 표현을 전담하는 클래스를 정의
*
*/

namespace RollingBall
{
	class PaintManager
	{
	private:
		struct {
			HINSTANCE hInstance;
			HWND hwnd;
			PAINTSTRUCT ps;
			RECT windowRect;

			struct {
				HDC window;
				//HDC current;
				struct {
					HDC windowBuffer;
					struct {
						HDC background;
						HDC ball;
						HDC ball_mask;
					} res;
				} mem;
			} hDC;

			struct {
				HBITMAP windowBuffer;
				struct {
					HBITMAP background;
					HBITMAP ball;
					HBITMAP ball_mask;
				} res;
				struct {
					HBITMAP windowBuffer;
					struct {
						HBITMAP background;
						HBITMAP ball;
						HBITMAP ball_mask;
					} res;
				} old;
			} hBitmap;
		} winAPI;
		
		struct {
			BOOL isHDCwindowMode_GetDC;

			BOOL isSetMemDCres;
			BOOL isSetHBitmapRes;

			BOOL isDoubleBufferingStart;
			BOOL isInit;
			BOOL isInitBitmapManager;
		} flag;

		BitmapManager bitmapManager;
		int BallSizeType;
	
	public:
		~PaintManager();

		//PrintManager 클래스 변수를 사용하기 전 반드시 수행해야 함
		void init(HINSTANCE hInstance, HWND hwnd, int m_BallSizeType = BallSize_medium);
		void init_bitmapManager();

		//페인트를 시작한다
		void beginPaint();
		//페인트를 마친다
		void endPaint();

		//오브젝트를 페인트한다
		void paint_background();
		void paint_ball(int posX, int posY);
	
	
	private:

		void init_flags();

		//BOOL 멤버 변수 값을 리턴한다
		BOOL isHDCwindowMode_GetDC();
		BOOL isHDCwindowMode_BeginPaint();

		BOOL isSetHDCwindow();
		BOOL isSetMemDCwindowBuffer();
		BOOL isSetMemDCres();
		BOOL isSetHBitmapWindowBuffer();
		BOOL isSetHBitmapRes();

		BOOL isDoubleBufferingStart();
		BOOL isInit();
		BOOL isInitBitmapManager();
		BOOL isReadyToPaint();


		//hDC.window를 BeginPaint로 얻고 반납하도록 설정한다
		void hDCwindowMode_set_BeginPaint();
		//hDC.window를 GetDC로 얻고 반납하도록 설정한다
		void hDCwindowMode_set_GetDC();


		//hDC.window를 설정한 모드에 따라 얻는다
		void hDCwindow_set();
		//hDC.window를 설정한 모드에 따라 반납한다
		void hDCwindow_release();
		//hDC.window를 초기화한다
		void hDCwindow_init();
		

		//더블버퍼링을 시작한다.
		void doubleBuffering_start();
		//더블버퍼링을 종료한다.
		void doubleBuffering_stop();
		
		//hDC.mem 요소를 관리하는 함수이다
		void memDC_windowBuffer_set();
		void memDC_windowBuffer_release();
		void memDC_windowBuffer_init();
		void memDC_res_set();
		void memDC_res_release();
		void memDC_res_init();


		//멤버 변수를 설정한다
		void hBitmap_windowBuffer_init();
		void hBitmap_windowBuffer_set();
		void hBitmap_windowBuffer_release();
		void hBitmap_res_init();
		void hBitmap_res_set(int BallSizeType);
		void set_BallSizeType(int BallSize);


		//오브젝트를 버퍼에 그린다.
		void paint_background_ruller_tobuffer();
		void paint_background_tobuffer();
		void paint_ball_tobuffer(int x, int y);


		//버퍼에 그려진 그림을 윈도우로 출력한다.
		void flush_buffer();
	};
}

#endif