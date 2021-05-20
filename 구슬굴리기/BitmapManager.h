#pragma once
#ifndef __BitmapManager_h__
#define __BitmapManager_h__

//오브젝트 개수(floor, ball, ...)
#define BITMAPMANAGER_OBJECT_COUNT				2

//공 텍스쳐 개수(iron1, iron2, ...)
#define BITMAPMANAGER_BALL_TEXTURE_COUNT		1

//공 텍스쳐 사이즈 개수(032, 064, ...)
#define BITMAPMANAGER_BALL_TEXTURE_SIZE_COUNT	4

//바닥 텍스쳐 개수(wood1, ...)
#define BITMAPMANAGER_FLOOR_TEXTURE_COUNT		1

//바닥 텍스쳐 사이즈 개수(032, 064, ...)
#define BITMAPMANAGER_FLOOR_TEXTURE_SIZE_COUNT	1

//bmp 파일 개수
#define BITMAPMANAGER_BITMAP_FILE_COUNT			\
	2 * BITMAPMANAGER_BALL_TEXTURE_COUNT		\
	* BITMAPMANAGER_BALL_TEXTURE_SIZE_COUNT		\
	+ BITMAPMANAGER_FLOOR_TEXTURE_COUNT			\
	* BITMAPMANAGER_FLOOR_TEXTURE_SIZE_COUNT

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
#include <tchar.h>
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
				//오브젝트 개수(floor, ball, ...)
		const UINT object_count = 2;

		//공 텍스쳐 개수(iron1, iron2, ...)
		const UINT  ball_texture_count = 1;

		//공 텍스쳐 사이즈 개수(032, 064, ...)
		const UINT ball_texture_size_count = 4;

		//바닥 텍스쳐 개수(wood1, ...)
		const UINT floor_texture_count = 1;

		//바닥 텍스쳐 사이즈 개수(032, 064, ...)
		const UINT floor_texture_size_count = 1;

		const UINT bitmap_file_count = 
			2 * ball_texture_count * ball_texture_size_count +
			floor_texture_count * floor_texture_size_count;

		//필요없는 변수
		static BOOL isLoadedHBitmap;

		static HINSTANCE hInstance;
		static HBITMAP bitmap[BITMAPMANAGER_BITMAP_FILE_COUNT];
		const UINT BMPFILEMACRO[BITMAPMANAGER_BITMAP_FILE_COUNT] = {
			IDB_BALL_IRON1_032, IDB_BALL_IRON1_064, IDB_BALL_IRON1_128, IDB_BALL_IRON1_256,
			IDB_BALL_IRON1_032M, IDB_BALL_IRON1_064M, IDB_BALL_IRON1_128M, IDB_BALL_IRON1_256M,
			IDB_FLOOR_WOOD1_256
		};
		

		//old variables
		static HBITMAP floor;
		static HBITMAP ball[BallSizeCount];
		static HBITMAP ball_mask[BallSizeCount];
		int BallSizeType;

	public:
		struct {
			const int bmp_files = BITMAPMANAGER_BITMAP_FILE_COUNT;
			const int object_count = BITMAPMANAGER_OBJECT_COUNT;
			struct {
				struct {
					const int texture = BITMAPMANAGER_BALL_TEXTURE_COUNT;
					const int texture_size = BITMAPMANAGER_BALL_TEXTURE_SIZE_COUNT;
				} ball;
				struct {
					const int texture = BITMAPMANAGER_FLOOR_TEXTURE_COUNT;
					const int texture_size = BITMAPMANAGER_FLOOR_TEXTURE_SIZE_COUNT;
				} floor;
			} object;
		} count;

		~BitmapManager();
		void init(HINSTANCE m_hInstance, int m_BallSizeType = BallSize_medium);



		HBITMAP get(int index)
		{
			return bitmap[index];
		}

		int index(int object = 0, int texture = 0, int size = 32, BOOL mask = FALSE);
		int object(LPCTSTR object);
		int texture(LPCTSTR texture);

		HBITMAP operator[](int index)
		{
			return get(index);
		}

		//old functions
		HBITMAP get_hBitmap_floor();
		HBITMAP get_hBitmap_ball();
		HBITMAP get_hBitmap_ball_mask();		
		void set_BallSizeType(int m_BallSizeType);
		int get_BallSizeType();


	private:
		int BallSize_toIdx(int BallSize);
	};

}

#endif