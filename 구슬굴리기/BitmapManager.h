#pragma once
#ifndef __BitmapManager_h__
#define __BitmapManager_h__
#include <Windows.h>
#include "resource.h"

/*
* 
* BitmapManager:
* ���α׷����� ����ϴ� ��Ʈ���� �����ϴ� Ŭ������ ����
* 
*/


namespace RollingBall
{
	const static int BallSize_small = 32;
	const static int BallSize_medium = 64;
	const static int BallSize_large = 128;
	const static int BallSize_extra = 256;

	class BitmapManager
	{
	public:
		//���� ��Ʈ�� �ؽ��� ������ �ؽ��� ũ�⸦ �����Ѵ�.
		const static int BallSizeCount = 4;

		//�ٴ� �ؽ��� ũ�⸦ �����Ѵ�.
		const static int FloorSize = 256;
	private:
		HBITMAP floor;
		HBITMAP ball[BallSizeCount];
		HBITMAP ball_mask[BallSizeCount];
		HINSTANCE hInstance;

	public:
		void initialize(HINSTANCE m_hInstance);

		HBITMAP get_hBitmap_floor();
		HBITMAP get_hBitmap_ball(int BallSize);
		HBITMAP get_hBitmap_ball_mask(int BallSize);
	private:
		int BallSize_toIdx(int BallSize);
	};


}


#endif