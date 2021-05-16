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

		int BallSizeType;

		//30px를 1cm로 정의할 수 있다.
		void show_background_ruller();

	public:
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);

		void set_BallSizeType(int BallSize);

		void show_background(LPRECT region);
		void show_ball(int x, int y);
	};
}
#endif