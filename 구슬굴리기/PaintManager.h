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
			BOOL isWindowDCmode_GetDC;
			BOOL isSetWindowDC;
			BOOL isSetMemoryDC;
		} flag;

		BitmapManager bitmapManager;
		int BallSizeType;
	
	public:
		~PaintManager();

		//PrintManager 클래스 변수를 사용하기 전 반드시 수행해야 함
		void initialize(HINSTANCE hInstance, HWND hwnd, int m_BallSizeType = BallSize_medium);

		//페인트를 시작한다
		void beginPaint();
		//페인트를 마친다
		void endPaint();

		//오브젝트를 페인트한다
		void paint_background();
		void paint_ball(int posX, int posY);
	
	
	private:
		//BOOL 멤버 변수 값을 리턴한다
		BOOL isWindowDCmode_GetDC();
		BOOL isSetWindowDC();
		BOOL isSetMemoryDC();
		BOOL isAbleToPrint();


		//hDC.window를 BeginPaint로 얻고 반납하도록 설정한다
		void set_windowDCmode_BeginPaint();
		//hDC.window를 GetDC로 얻고 반납하도록 설정한다
		void set_windowDCmode_GetDC();


		//hDC.window를 설정한 모드에 따라 얻는다
		void set_hDCwindow();
		//hDC.window를 설정한 모드에 따라 반납한다
		void release_hDCwindow();


		//hDC.memory들을 얻고 설정한다.
		void set_memoryDC();
		//hDC.memory들을 삭제한다.
		void release_memoryDC();


		//멤버 변수를 설정한다
		void set_hBitmap(int BallSizeType);
		void set_BallSizeType(int BallSize);


		//오브젝트를 버퍼에 그린다.
		void paint_background_ruller_tobuffer();
		void paint_background_tobuffer(LPRECT region);
		void paint_ball_tobuffer(int x, int y);


		//버퍼에 그려진 그림을 윈도우로 출력한다.
		void flush_buffer();
	};
}

#endif