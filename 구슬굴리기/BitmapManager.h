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
		static int object_count;

		static HINSTANCE hInstance;
		static HWND hwnd;

		static HBITMAP floor;
		static HBITMAP ball[BallSizeCount];
		static HBITMAP ball_mask[BallSizeCount];

		HBITMAP hDCwindowBuffer;
		int BallSizeType;

	public:
		BitmapManager();
		~BitmapManager();
		void initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = BallSize_medium);
		BOOL isInitialized();

		void set_BallSizeType(int m_BallSizeType);
		void set_hDCwindowBuffer();

		HBITMAP get_hBitmap_floor();
		HBITMAP get_hBitmap_ball();
		HBITMAP get_hBitmap_ball_mask();
		HBITMAP get_hBitmap_hDCwindowBuffer();
		int get_BallSizeType();

		void release_hDCwindowBuffer();
	private:
		int BallSize_toIdx(int BallSize);
	};
}

#endif