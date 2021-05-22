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
#include <string>
#include <vector>
#include "resource.h"

using std::vector;
typedef std::basic_string<TCHAR> tstring;

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
		static int _object_count;
		struct BitmapManagerObject {
			tstring name;
			BOOL has_mask;
			struct {
				int count;
				vector<tstring> name;
				struct {
					int count;
					vector<int> val;
				} size;
			} texture;
		};

		//오브젝트에 대한 정보를 담은 벡터
		vector<BitmapManagerObject> _object;


		/******************************************************
		*	private variables: const static(파일 개수 관련)
		*******************************************************/
		//오브젝트 개수(ball, floor,  ...)
		const static UINT object_count = 2;

		//오브젝트:공 텍스쳐 개수(iron1, iron2, ...) 및
		//오브젝트:공 텍스쳐 사이즈 개수(032, 064, ...)
		const static UINT  ball_texture_count = 1;
		const static UINT ball_texture_size_count = 4;
		const static UINT flag_ball_has_mask = TRUE;

		//오브젝트:바닥 텍스쳐 개수(wood1, ...) 및
		//오브젝트:바닥 텍스쳐 사이즈 개수(032, 064, ...)
		const static UINT floor_texture_count = 1;
		const static UINT floor_texture_size_count = 1;
		const static UINT flag_floor_has_mask = FALSE;

		//새 오브젝트 비트맵 파일을 추가할 시 수정
		//한 줄에 하나의 오브젝트 비트맵 파일 정보를 담고 있음
		//{오브젝트 텍스쳐 개수, 오브젝트 텍스쳐 크기 개수, 오브젝트 마스크 비트맵 존재여부}
		const UINT count_table[object_count][3] = {
			{ ball_texture_count, ball_texture_size_count, flag_ball_has_mask },
			{ floor_texture_count, floor_texture_size_count, flag_floor_has_mask }
		};
		//새 오브젝트 비트맵 파일을 추가할 시 수정
		//한 줄에 오브젝트 비트맵 파일의 각 크기값을 담고 있음
		const int texture_size_table
			[ball_texture_size_count 
			+ floor_texture_size_count] = {
				32, 64, 128, 256,
				256
		};

		enum {_texture = 0, _size = 1, _mask = 2};

		/*******************************
		*	private variables: static
		********************************/
		//새 비트맵 리소스를 추가시 수정
		//BMPFILEMACRO의 실질적 길이는 bitmap_file_count임
		const UINT BMPFILEMACRO[256] = {
			IDB_BALL_IRON1_032, IDB_BALL_IRON1_064, IDB_BALL_IRON1_128, IDB_BALL_IRON1_256,
			IDB_BALL_IRON1_032M, IDB_BALL_IRON1_064M, IDB_BALL_IRON1_128M, IDB_BALL_IRON1_256M,
			IDB_FLOOR_WOOD1_256
		};
		//로드된 hBitmap들을 담을 동적 배열
		static HBITMAP* hBitmap;
		//비트맵 파일 개수: init 함수 호출시 자동으로 초기화
		static UINT bitmap_file_count;
		static HINSTANCE hInstance;
		

		//old variables
		//static BOOL isLoadedHBitmap;
		static HBITMAP oldvar_floor;
		static HBITMAP oldvar_ball[BallSizeCount];
		static HBITMAP oldvar_ball_mask[BallSizeCount];
		int BallSizeType;



		/************************************
		*	private functions
		*************************************/
		//bitmap_file_count를 초기화하는 함수
		void init_bitmap_file_count();
		//hBitmap 배열을 동적 생성하고 hBitmap을 로드하는 함수
		void init_hBitmap();
		//hBitmap을 삭제하고 동적 할당 해제하는 함수
		void delete_hBitmap();

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

		void init(HINSTANCE m_hInstance, int m_BallSizeType = BallSize_medium);
		BOOL isInit();
		~BitmapManager();


		HBITMAP get(int index);

		int index(int object = 0, int texture = 0, int size = 32, BOOL mask = FALSE);
		int object(LPCTSTR object);
		int texture(LPCTSTR texture);
		int size(int size);

		//get() 함수를 호출한다
		HBITMAP operator[](int index);

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