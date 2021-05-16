#pragma once
#ifndef __PrintManager_h__
#define __PrintManager_h__
#include <Windows.h>
#include "BitmapManager.h"

/*
*
* PrintManager:
* 프로그램의 시각적 표현을 전담하는 클래스를 정의
*
*/

namespace RollingBall
{
	class PrintManager
	{
	private:
		BitmapManager bitmapManager;

		
		HINSTANCE hInstance;
		HWND hwnd;
		PAINTSTRUCT ps;
		RECT windowRect;

		struct {
			HDC window;
			HDC current;
			struct {
				HDC hDCwindowCompatible;
				HDC background;
				HDC ball;
				HDC ball_mask;
			} memory;
		} hDC;

		struct {
			HBITMAP hDCwindowCompatible;
			struct {
				HBITMAP background;
				HBITMAP ball;
				HBITMAP ball_mask;
			} resource;
			struct {
				HBITMAP hDCwindowCompatible;
				HBITMAP background;
				HBITMAP ball;
				HBITMAP ball_mask;
			} holdingOld;
		} hBitmap;

		BOOL m_isWindowDCmode_GetDC;
		BOOL m_isSetWindowDC;
		BOOL m_isSetMemoryDC;

		int BallSizeType;

	private:
		//hDC.window를 설정한 모드에 따라 얻는다
		void set_hDCwindow();
		//hDC.window를 설정한 모드에 따라 반납한다
		void release_hDCwindow();


		//hDC.memory들을 얻고 설정한다.
		void set_memoryDC();
		//hDC.memory들을 삭제한다.
		void release_memoryDC();

	public:
		//PrintManager 클래스 변수를 사용하기 전 반드시 수행해야 함
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);

		//hDC.window를 BeginPaint로 얻고 반납하도록 설정한다
		void set_windowDCmode_BeginPaint();
		//hDC.window를 GetDC로 얻고 반납하도록 설정한다
		void set_windowDCmode_GetDC();

		//HDC 변수들을 통합적으로 설정하고 반납한다
		void set_hDC();
		void release_hDC();

		//멤버 변수를 설정한다
		void set_hBitmap(int BallSizeType);
		void set_BallSizeType(int BallSize);

		//BOOL 멤버 변수 값을 리턴한다
		BOOL isWindowDCmode_GetDC();
		BOOL isSetWindowDC();
		BOOL isSetMemoryDC();
		BOOL isAbleToPrint();

		//30px를 1cm로 정의할 수 있다.
		void draw_background_ruller_tobuffer();
		void draw_background_tobuffer(LPRECT region);
		void draw_ball_tobuffer(int x, int y);

		void flush_buffer();

		~PrintManager();
	};
}

#endif