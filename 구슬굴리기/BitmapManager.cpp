#include "BitmapManager.h"

using namespace RollingBall;


void BitmapManager::initialize(HINSTANCE m_hInstance)
{
	//∫Ò∆Æ∏  ∑ŒµÂ
	hInstance = m_hInstance;
	floor = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLOOR));
	for (int size = 0; size < BallSizeCount; size++)
	{
		ball[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BALL_032 + 2 * size));
		ball_mask[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BALL_032_MASK + 2 * size));
	}
}

int BitmapManager::BallSize_toIdx(int BallSize)
{
	switch (BallSize)
	{
	case BallSize_small:
		return 0;
	case BallSize_medium:
		return 1;
	case BallSize_large:
		return 2;
	case BallSize_extra:
		return 3;
	default:
		return -1;
	}		
}

HBITMAP BitmapManager::get_hBitmap_floor()
{
	return floor;
}

HBITMAP BitmapManager::get_hBitmap_ball(int BallSize)
{
	return ball[BallSize_toIdx(BallSize)];
}

HBITMAP BitmapManager::get_hBitmap_ball_mask(int BallSize)
{
	return ball_mask[BallSize_toIdx(BallSize)];
}