#include "BitmapManager.h"

using namespace RollingBall;

int BitmapManager::isLoadedBitmap = FALSE;
HINSTANCE BitmapManager::hInstance = NULL;
HBITMAP BitmapManager::floor = NULL;
HBITMAP BitmapManager::ball[BallSizeCount] = { NULL, };
HBITMAP BitmapManager::ball_mask[BallSizeCount] = { NULL, };

BitmapManager::~BitmapManager()
{
	if (isLoadedBitmap)
	{
		hInstance = NULL;
		DeleteObject(floor);
		for (int size = 0; size < BallSizeCount; size++)
		{
			DeleteObject(ball[size]);
			DeleteObject(ball_mask[size]);
		}
	}
	isLoadedBitmap = FALSE;
}

void BitmapManager::init(HINSTANCE m_hInstance, int m_BallSizeType)
{
	if (!isLoadedBitmap)
	{
		//��Ʈ�� �ε�
		hInstance = m_hInstance;
		floor = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLOOR));
		for (int size = 0; size < BallSizeCount; size++)
		{
			ball[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BALL_032 + 2 * size));
			ball_mask[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BALL_032_MASK + 2 * size));
		}
	}
	set_BallSizeType(m_BallSizeType);

	isLoadedBitmap = TRUE;
}

void BitmapManager::set_BallSizeType(int m_BallSizeType)
{
	BallSizeType = m_BallSizeType;
}

HBITMAP BitmapManager::get_hBitmap_floor()
{
	return floor;
}

HBITMAP BitmapManager::get_hBitmap_ball()
{
	return ball[BallSize_toIdx(BallSizeType)];
}

HBITMAP BitmapManager::get_hBitmap_ball_mask()
{
	return ball_mask[BallSize_toIdx(BallSizeType)];
}

int BitmapManager::get_BallSizeType()
{
	return BallSizeType;
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
