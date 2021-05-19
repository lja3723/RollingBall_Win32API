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
			BOOL isWindowDCmode_GetDC;
			BOOL isSetWindowDC;
			BOOL isSetMemoryDC;
		} flag;

		BitmapManager bitmapManager;
		int BallSizeType;
	
	public:
		~PaintManager();

		//PrintManager Ŭ���� ������ ����ϱ� �� �ݵ�� �����ؾ� ��
		void initialize(HINSTANCE hInstance, HWND hwnd, int m_BallSizeType = BallSize_medium);

		//����Ʈ�� �����Ѵ�
		void beginPaint();
		//����Ʈ�� ��ģ��
		void endPaint();

		//������Ʈ�� ����Ʈ�Ѵ�
		void paint_background();
		void paint_ball(int posX, int posY);
	
	
	private:
		//BOOL ��� ���� ���� �����Ѵ�
		BOOL isWindowDCmode_GetDC();
		BOOL isSetWindowDC();
		BOOL isSetMemoryDC();
		BOOL isAbleToPrint();


		//hDC.window�� BeginPaint�� ��� �ݳ��ϵ��� �����Ѵ�
		void set_windowDCmode_BeginPaint();
		//hDC.window�� GetDC�� ��� �ݳ��ϵ��� �����Ѵ�
		void set_windowDCmode_GetDC();


		//hDC.window�� ������ ��忡 ���� ��´�
		void set_hDCwindow();
		//hDC.window�� ������ ��忡 ���� �ݳ��Ѵ�
		void release_hDCwindow();


		//hDC.memory���� ��� �����Ѵ�.
		void set_memoryDC();
		//hDC.memory���� �����Ѵ�.
		void release_memoryDC();


		//��� ������ �����Ѵ�
		void set_hBitmap(int BallSizeType);
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