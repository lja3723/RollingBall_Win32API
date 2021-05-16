#pragma once
#ifndef __BitmapManager_h__
#define __BitmapManager_h__
#include <Windows.h>
#include "resource.h"

/*
* 
* BitmapManager:
* 프로그램에서 사용하는 비트맵을 관리하는 클래스를 정의
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
		//구슬 비트맵 텍스쳐 개수와 텍스쳐 크기를 정의한다.
		const static int BallSizeCount = 4;

		//바닥 텍스쳐 크기를 정의한다.
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