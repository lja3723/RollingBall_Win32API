#pragma once
#ifndef __BitmapManager_h__
#define __BitmapManager_h__

//구슬 크기의 종류
#define BallSizeCount 4

//구슬 크기 종류의 각 크기
#define BallSize_small 32
#define BallSize_medium 64
#define BallSize_large 128
#define BallSize_extra 256

//바닥 텍스쳐 크기
#define FloorSize = 256

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

	class BitmapManager
	{
	private:
		static BOOL isLoadedBitmap;
		static HINSTANCE hInstance;
		static HBITMAP floor;
		static HBITMAP ball[BallSizeCount];
		static HBITMAP ball_mask[BallSizeCount];

		int BallSizeType;

	public:
		~BitmapManager();
		void init(HINSTANCE m_hInstance, int m_BallSizeType = BallSize_medium);

		void set_BallSizeType(int m_BallSizeType);
		int get_BallSizeType();

		HBITMAP get_hBitmap_floor();
		HBITMAP get_hBitmap_ball();
		HBITMAP get_hBitmap_ball_mask();
	private:
		int BallSize_toIdx(int BallSize);
	};
}

#endif