#pragma once
#ifndef __PrintManage_h__
#define __PrintManage_h__
#include <Windows.h>
#include "BitmapManage.h"
#include "PhysicsControl.h"

/*
*
* PrintManage:
* 프로그램의 시각적 표현을 전담하는 클래스를 정의
*
*/

namespace RollingBall
{
	class PrintManager
	{
	private:
		BitmapManager bmpManager;
		HINSTANCE hInstance;
		HWND hwnd;

		int BallSizeType;

		//30px를 1cm로 정의할 수 있다.
		void print_background_ruller();

	public:
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);

		void set_BallSizeType(int BallSize);

		void print_background(LPRECT region);
		void print_ball(int x, int y);
	};
}
#endif