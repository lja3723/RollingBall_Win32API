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
			BOOL isSetHDCwindow;
			BOOL isSetMemoryDC;
			BOOL isInit;
			BOOL isInitBitmapManager;
		} flag;

		BitmapManager bitmapManager;
		int BallSizeType;
	
	public:
		~PaintManager();

		//PrintManager Ŭ���� ������ ����ϱ� �� �ݵ�� �����ؾ� ��
		void init(HINSTANCE hInstance, HWND hwnd, int m_BallSizeType = BallSize_medium);
		void init_bitmapManager();

		//����Ʈ�� �����Ѵ�
		void beginPaint();
		//����Ʈ�� ��ģ��
		void endPaint();

		//������Ʈ�� ����Ʈ�Ѵ�
		void paint_background();
		void paint_ball(int posX, int posY);
	
	
	private:

		void init_flags();

		//BOOL ��� ���� ���� �����Ѵ�
		BOOL isHDCwindowMode_GetDC();
		BOOL isHDCwindowMode_BeginPaint();
		BOOL isSetHDCwindow();
		BOOL isSetMemoryDC();
		BOOL isInit();
		BOOL isInitBitmapManager();
		BOOL isReadyToPaint();


		//hDC.window�� BeginPaint�� ��� �ݳ��ϵ��� �����Ѵ�
		void hDCwindowMode_set_BeginPaint();
		//hDC.window�� GetDC�� ��� �ݳ��ϵ��� �����Ѵ�
		void hDCwindowMode_set_GetDC();


		//hDC.window�� ������ ��忡 ���� ��´�
		void hDCwindow_set();
		//hDC.window�� ������ ��忡 ���� �ݳ��Ѵ�
		void hDCwindow_release();
		//hDC.window�� �ʱ�ȭ�Ѵ�
		void hDCwindow_init();
		

		//hDC.memory���� ��� �����Ѵ�.
		void memoryDC_set();
		//hDC.memory���� �����Ѵ�.
		void memoryDC_release();
		//hDC.memory���� �ʱ�ȭ�Ѵ�.
		void memoryDC_init();

		//��� ������ �����Ѵ�
		void set_hBitmap_res(int BallSizeType);
		void set_BallSizeType(int BallSize);


		//������Ʈ�� ���ۿ� �׸���.
		void paint_background_ruller_tobuffer();
		void paint_background_tobuffer(LPRECT region);
		void paint_ball_tobuffer(int x, int y);


		//���ۿ� �׷��� �׸��� ������� ����Ѵ�.
		void flush_buffer();
	};
}

#endif