#pragma once
#ifndef __PrintManager_h__
#define __PrintManager_h__
#include <Windows.h>
#include "BitmapManager.h"

/*
*
* PrintManager:
* ���α׷��� �ð��� ǥ���� �����ϴ� Ŭ������ ����
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
		//hDC.window�� ������ ��忡 ���� ��´�
		void set_hDCwindow();
		//hDC.window�� ������ ��忡 ���� �ݳ��Ѵ�
		void release_hDCwindow();


		//hDC.memory���� ��� �����Ѵ�.
		void set_memoryDC();
		//hDC.memory���� �����Ѵ�.
		void release_memoryDC();

	public:
		//PrintManager Ŭ���� ������ ����ϱ� �� �ݵ�� �����ؾ� ��
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);

		//hDC.window�� BeginPaint�� ��� �ݳ��ϵ��� �����Ѵ�
		void set_windowDCmode_BeginPaint();
		//hDC.window�� GetDC�� ��� �ݳ��ϵ��� �����Ѵ�
		void set_windowDCmode_GetDC();

		//HDC �������� ���������� �����ϰ� �ݳ��Ѵ�
		void set_hDC();
		void release_hDC();

		//��� ������ �����Ѵ�
		void set_hBitmap(int BallSizeType);
		void set_BallSizeType(int BallSize);

		//BOOL ��� ���� ���� �����Ѵ�
		BOOL isWindowDCmode_GetDC();
		BOOL isSetWindowDC();
		BOOL isSetMemoryDC();
		BOOL isAbleToPrint();

		//30px�� 1cm�� ������ �� �ִ�.
		void draw_background_ruller_tobuffer();
		void draw_background_tobuffer(LPRECT region);
		void draw_ball_tobuffer(int x, int y);

		void flush_buffer();

		~PrintManager();
	};
}

#endif