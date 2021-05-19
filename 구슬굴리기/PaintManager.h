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
		BOOL isSetMemDCwindowBuffer();
		BOOL isSetMemDCres();
		BOOL isSetHBitmapWindowBuffer();
		BOOL isSetHBitmapRes();

		BOOL isDoubleBufferingStart();
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
		

		//������۸��� �����Ѵ�.
		void doubleBuffering_start();
		//������۸��� �����Ѵ�.
		void doubleBuffering_stop();
		
		//hDC.mem ��Ҹ� �����ϴ� �Լ��̴�
		void memDC_windowBuffer_set();
		void memDC_windowBuffer_release();
		void memDC_windowBuffer_init();
		void memDC_res_set();
		void memDC_res_release();
		void memDC_res_init();


		//��� ������ �����Ѵ�
		void hBitmap_windowBuffer_init();
		void hBitmap_windowBuffer_set();
		void hBitmap_windowBuffer_release();
		void hBitmap_res_init();
		void hBitmap_res_set(int BallSizeType);
		void set_BallSizeType(int BallSize);


		//������Ʈ�� ���ۿ� �׸���.
		void paint_background_ruller_tobuffer();
		void paint_background_tobuffer();
		void paint_ball_tobuffer(int x, int y);


		//���ۿ� �׷��� �׸��� ������� ����Ѵ�.
		void flush_buffer();
	};
}

#endif