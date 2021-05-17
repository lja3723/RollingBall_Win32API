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
		static BOOL isLoadedBitmap;
		static HINSTANCE hInstance;
		static HBITMAP floor;
		static HBITMAP ball[BallSizeCount];
		static HBITMAP ball_mask[BallSizeCount];

		int BallSizeType;

	public:
		~BitmapManager();
		void initialize(HINSTANCE m_hInstance, int m_BallSizeType = BallSize_medium);

		void set_BallSizeType(int m_BallSizeType);

		HBITMAP get_hBitmap_floor();
		HBITMAP get_hBitmap_ball();
		HBITMAP get_hBitmap_ball_mask();
		int get_BallSizeType();
	private:
		int BallSize_toIdx(int BallSize);
	};
}

#endif