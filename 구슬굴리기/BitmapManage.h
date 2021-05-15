#pragma once
#ifndef __BitmapManage_h__
#define __BitmapManage_h__
#include <Windows.h>
#include "resource.h"

/*
* 
* BitmapManage:
* 프로그램에서 사용하는 비트맵을 관리하는 클래스를 정의
* 
*/


namespace RollingBall
{
	//구슬 비트맵 텍스쳐 크기 종류를 명시한다.
	const static int BallSizeTypes = 4;

	//구슬 비트맵 텍스쳐 크기 종류의 각 크기를 명시한다.
	const static int BallSize_small = 32;
	const static int BallSize_medium = 64;
	const static int BallSize_large = 128;
	const static int BallSize_extra = 256;

	//구슬 그림자 비트맵의 각 종류의 크기를 명시한다.
	const static int BallSize_small_shadow = 36;
	const static int BallSize_medium_shadow = 72;
	const static int BallSize_large_shadow = 143;
	const static int BallSize_extra_shadow = 286;

	//바닥 텍스쳐 크기를 명시한다.
	const static int FloorSize = 256;

	class BitmapManager
	{
	public:
	private:
		HBITMAP floor;
		HBITMAP ball[BallSizeTypes];
		HBITMAP ball_mask[BallSizeTypes];
		HBITMAP ball_shadow[BallSizeTypes];
		HINSTANCE hInstance;

	public:
		void initialize(HINSTANCE m_hInstance);

		HBITMAP get_hBitmap_floor();
		HBITMAP get_hBitmap_ball(int BallSize);
		HBITMAP get_hBitmap_ball_mask(int BallSize);
		HBITMAP get_hBitmap_ball_shadow(int BallSize);
	private:
		int BallSize_toIdx(int BallSize);
	};


}

#endif