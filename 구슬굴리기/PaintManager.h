#pragma once
#ifndef __PaintManager_h__
#define __PaintManager_h__
#include <Windows.h>
#include "BitmapManager.h"

/*
*
* PaintManager:
* ���α׷��� �ð��� ǥ���� �����ϴ� Ŭ������ ����
*
*/

namespace RollingBall
{
	class PaintManager
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
		//hDC.window�� ������ ��忡 ���� ��´�
		void set_hDCwindow();
		//hDC.window�� ������ ��忡 ���� �ݳ��Ѵ�
		void release_hDCwindow();


		//hDC.memory���� ��� �����Ѵ�.
		void set_memoryDC();
		//hDC.memory���� �����Ѵ�.
		void release_memoryDC();

		//BOOL ��� ���� ���� �����Ѵ�
		BOOL isWindowDCmode_GetDC();
		BOOL isSetWindowDC();
		BOOL isSetMemoryDC();
		BOOL isAbleToPrint();

		//hDC.window�� BeginPaint�� ��� �ݳ��ϵ��� �����Ѵ�
		void set_windowDCmode_BeginPaint();
		//hDC.window�� GetDC�� ��� �ݳ��ϵ��� �����Ѵ�
		void set_windowDCmode_GetDC();

		//��� ������ �����Ѵ�
		void set_hBitmap(int BallSizeType);
		void set_BallSizeType(int BallSize);

		//30px�� 1cm�� ������ �� �ִ�.
		void draw_background_ruller_tobuffer();
		void draw_background_tobuffer(LPRECT region);
		void draw_ball_tobuffer(int x, int y);

		void flush_buffer();


	public:
		//PrintManager Ŭ���� ������ ����ϱ� �� �ݵ�� �����ؾ� ��
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);

		//����Ʈ�� �����Ѵ�
		void beginPaint();

		//����Ʈ�Ѵ�.
		void paint_background();
		void paint_ball(int posX, int posY);

		//����Ʈ�� ��ģ��
		void endPaint();
		

		

		
		~PaintManager();
	};
}

#endif