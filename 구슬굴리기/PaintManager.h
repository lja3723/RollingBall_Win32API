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

		//Win32API���� ���Ǵ� �������� ����ü
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
		
		//�������� �÷��׸� �����ϴ� ����
		struct {
			BOOL isHDCwindowMode_GetDC;

			BOOL isSetMemDCres;
			BOOL isSetHBitmapRes;
			BOOL isBackedUpHBitmapRes;

			BOOL isDoubleBufferingStart;
			BOOL isInit;
			BOOL isInitBitmapManager;
		} flag;
	
	public:
		~PaintManager();

		//PrintManager Ŭ���� ������ ����ϱ� �� �ݵ�� �����ؾ� �Ѵ�
		void init(HINSTANCE hInstance, HWND hwnd, int BallSizeType = BallSize_medium);

		//����Ʈ�� �����Ѵ�
		void beginPaint();

		//����Ʈ�� ��ģ��
		void endPaint();

		//����� ����Ʈ�Ѵ�
		void paint_background();

		//���� ����Ʈ�Ѵ�
		void paint_ball(int posX, int posY);
	
	
	private:
		/********************************
		*
		*	init management
		*
		*********************************/
		//Ŭ������ ���� �÷��׺����� �ʱ�ȭ
		//PaintManager::init()������ ȣ��Ǿ�� ��
		void init_flags();

		//BitmapManager ������ �ʱ�ȭ
		//PaintManager::init()������ ȣ��Ǿ�� ��
		void init_bitmapManager();



		/********************************
		*
		*	bool returns
		*
		*********************************/
		//���� �ʱ�ȭ ���θ� �˷���
		BOOL isInit();
		BOOL isInitBitmapManager();

		//hDC.window�� ��� ��尡 �������� �˷���
		BOOL isHDCwindowMode_GetDC();
		BOOL isHDCwindowMode_BeginPaint();

		//������ ���õǾ����� �˷���
		BOOL isSetHDCwindow();
		BOOL isSetMemDCwindowBuffer();
		BOOL isSetMemDCres();
		BOOL isSetHBitmapWindowBuffer();
		BOOL isSetHBitmapRes();
		BOOL isBackedUpHBitmapWindowBuffer();
		BOOL isBackedUpHBitmapRes();

		//��Ÿ ������ �˷���
		BOOL isDoubleBufferingStart();
		BOOL isReadyToPaint();



		/********************************
		*
		*	hDC.window management
		*
		*********************************/
		//hDC.window�� � ������� ������ ������
		//BeginPaint �Ǵ� GetDC���� ���� �� ����
		void hDCwindowMode_set_BeginPaint();
		void hDCwindowMode_set_GetDC();
	
		//hDC.window�� ������
		void hDCwindow_init();
		void hDCwindow_set();
		void hDCwindow_release();
		


		/********************************
		*
		*	hDC.mem management
		*
		*********************************/
		//hDC.mem ��Ҹ� ������
		void memDC_windowBuffer_init();
		void memDC_windowBuffer_set();
		void memDC_windowBuffer_release();

		void memDC_res_init();
		void memDC_res_set();
		void memDC_res_release();



		/********************************
		*
		*	hBitmap management
		*
		*********************************/
		//hBitmap ������ ������
		void hBitmap_windowBuffer_init();
		void hBitmap_windowBuffer_set();
		void hBitmap_windowBuffer_release();
		void hBitmap_res_init();
		void hBitmap_res_set();

		//hBitmap.old ������ ������
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
		//������۸��� �����ϰ� ������
		void doubleBuffering_start();
		void doubleBuffering_stop();



		/********************************
		*
		*	variable management
		*
		*********************************/
		//BallSizeType�� ������
		void set_BallSizeType(int m_BallSizeType);



		/********************************
		*
		*	paint management
		*
		*********************************/
		//������Ʈ�� ���ۿ� �׸�
		void paint_background_tobuffer();
		void paint_background_ruller_tobuffer();
		void paint_ball_tobuffer(int x, int y);

		//���ۿ� �׷��� �׸��� ������� ���
		void flush_buffer();
	};
}

#endif