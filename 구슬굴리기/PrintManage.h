#pragma once
#ifndef __PrintManage_h__
#define __PrintManage_h__
#include <Windows.h>
#include "BitmapManage.h"
#include "PhysicsControl.h"

/*
*
* PrintManage:
* ���α׷��� �ð��� ǥ���� �����ϴ� Ŭ������ ����
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

		//30px�� 1cm�� ������ �� �ִ�.
		void print_background_ruller();

	public:
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);

		void set_BallSizeType(int BallSize);

		void print_background(LPRECT region);
		void print_ball(int x, int y);
	};
}
#endif