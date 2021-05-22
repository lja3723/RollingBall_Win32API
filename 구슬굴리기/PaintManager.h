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
		BitmapManager bmp;

		//Win32API에서 사용되는 변수들의 집합체
		struct {
			HINSTANCE hInstance;
			HWND hwnd;
			PAINTSTRUCT ps;
			RECT windowRect;

			struct {
				HDC window;
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
		
		//여러가지 플래그를 저장하는 변수
		struct {
			BOOL isHDCwindowMode_GetDC;

			BOOL isSetMemDCres;
			BOOL isSetHBitmapRes;
			BOOL isBackedUpHBitmapRes;

			BOOL isDoubleBufferingStart;
			BOOL isInit;
			BOOL isInitDoubleBuffering;
		} flag;
	
	public:
		~PaintManager();

		//PrantManager 클래스 변수를 사용하기 전 반드시 수행해야 한다
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd, int BallSizeType = old_BallSize_medium);

		//페인트를 시작한다
		void beginPaint();

		//페인트를 마친다
		void endPaint();

		//배경을 페인트한다
		void paint_background();

		//공을 페인트한다
		void paint_ball(int posX, int posY);
	
	
	private:
		/********************************
		*
		*	init management
		*
		*********************************/
		//클래스의 각종 플래그변수를 초기화
		//PaintManager::init()에서만 호출되어야 함
		void init_flags();



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
		*	variable management
		*
		*********************************/
		//BallSizeType을 설정함
		void set_BallSizeType(int m_BallSizeType);



		/********************************
		*
		*	paint management
		*
		*********************************/
		//오브젝트를 버퍼에 그림
		void paint_background_tobuffer();
		void paint_background_ruller_tobuffer();
		void paint_ball_tobuffer(int x, int y);

		//버퍼에 그려진 그림을 윈도우로 출력
		void flush_buffer();
	};
}

#endif