#pragma once
#ifndef __BitmapManager_h__
#define __BitmapManager_h__

//���� ũ���� ����
#define BallSizeCount 4

//���� ũ�� ������ �� ũ��
#define BallSize_small 32
#define BallSize_medium 64
#define BallSize_large 128
#define BallSize_extra 256

//�ٴ� �ؽ��� ũ��
#define FloorSize = 256

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