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

		int BallSizeType;

		//30px�� 1cm�� ������ �� �ִ�.
		void show_background_ruller();

	public:
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);

		void set_BallSizeType(int BallSize);

		void show_background(LPRECT region);
		void show_ball(int x, int y);
	};
}
#endif