#pragma once
#ifndef __BitmapManager_h__
#define __BitmapManager_h__

//오브젝트 개수(floor, ball, ...)
#define old_BITMAPMANAGER_OBJECT_COUNT				2

//공 텍스쳐 개수(iron1, iron2, ...)
#define old_BITMAPMANAGER_BALL_TEXTURE_COUNT		1

//공 텍스쳐 사이즈 개수(032, 064, ...)
#define old_BITMAPMANAGER_BALL_TEXTURE_SIZE_COUNT	4

//바닥 텍스쳐 개수(wood1, ...)
#define old_BITMAPMANAGER_FLOOR_TEXTURE_COUNT		1

//바닥 텍스쳐 사이즈 개수(032, 064, ...)
#define old_BITMAPMANAGER_FLOOR_TEXTURE_SIZE_COUNT	1

//bmp 파일 개수
#define old_BITMAPMANAGER_BITMAP_FILE_COUNT			\
	2 * old_BITMAPMANAGER_BALL_TEXTURE_COUNT		\
	* old_BITMAPMANAGER_BALL_TEXTURE_SIZE_COUNT		\
	+ old_BITMAPMANAGER_FLOOR_TEXTURE_COUNT			\
	* old_BITMAPMANAGER_FLOOR_TEXTURE_SIZE_COUNT

//구슬 크기의 종류
#define old_BallSizeCount 4

//구슬 크기 종류의 각 크기
#define old_BallSize_small 32
#define old_BallSize_medium 64
#define old_BallSize_large 128
#define old_BallSize_extra 256

//바닥 텍스쳐 크기
#define old_FloorSize = 256

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
		struct BitmapManagerObject {
			tstring name;
			BOOL has_mask;
			struct {
				//int count;
				vector<tstring> name;
				vector<int> value;
			} texture;
		};

		//오브젝트에 대한 정보를 담은 벡터
		vector<BitmapManagerObject> object_info;


		/******************************************************
		*	private variables: const static(파일 개수 관련)
		*******************************************************/
		//오브젝트 개수(ball, floor,  ...)
		const static UINT old_object_count = 2;

		//오브젝트:공 텍스쳐 개수(iron1, iron2, ...) 및
		//오브젝트:공 텍스쳐 사이즈 개수(032, 064, ...)
		const static UINT  old_ball_texture_count = 1;
		const static UINT old_ball_texture_size_count = 4;
		const static UINT old_flag_ball_has_mask = TRUE;

		//오브젝트:바닥 텍스쳐 개수(wood1, ...) 및
		//오브젝트:바닥 텍스쳐 사이즈 개수(032, 064, ...)
		const static UINT old_floor_texture_count = 1;
		const static UINT old_floor_texture_size_count = 1;
		const static UINT old_flag_floor_has_mask = FALSE;

		//새 오브젝트 비트맵 파일을 추가할 시 수정
		//한 줄에 하나의 오브젝트 비트맵 파일 정보를 담고 있음
		//{오브젝트 텍스쳐 개수, 오브젝트 텍스쳐 크기 개수, 오브젝트 마스크 비트맵 존재여부}
		const UINT old_count_table[old_object_count][3] = {
			{ old_ball_texture_count, old_ball_texture_size_count, old_flag_ball_has_mask },
			{ old_floor_texture_count, old_floor_texture_size_count, old_flag_floor_has_mask }
		};
		//새 오브젝트 비트맵 파일을 추가할 시 수정
		//한 줄에 오브젝트 비트맵 파일의 각 크기값을 담고 있음
		const int old_texture_size_table
			[old_ball_texture_size_count 
			+ old_floor_texture_size_count] = {
				32, 64, 128, 256,
				256
		};

		enum {old__texture = 0, old__size = 1, old__mask = 2};

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
		static HBITMAP oldvar_ball[old_BallSizeCount];
		static HBITMAP oldvar_ball_mask[old_BallSizeCount];
		int BallSizeType;



		/************************************
		*	private functions
		*************************************/
		//object vector를 object_info.txt 기반으로 초기화하는 함수
		BOOL init_object_info(HWND hwnd);
		//파일에서 한 라인씩 읽어서 line에 저장해주는 함수
		BOOL ReadLine(HANDLE hFile, LPTSTR line, int lineLength);
		//bitmap_file_count를 초기화하는 함수
		void init_bitmap_file_count();
		//hBitmap 배열을 동적 생성하고 hBitmap을 로드하는 함수
		void init_hBitmap();
		//hBitmap을 삭제하고 동적 할당 해제하는 함수
		void delete_hBitmap();

	public:
		//BitmapManager 클래스 변수를 사용하기 전 반드시 수행해야 한다
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType = old_BallSize_medium);
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
		HBITMAP old_get_hBitmap_floor();
		HBITMAP old_get_hBitmap_ball();
		HBITMAP old_get_hBitmap_ball_mask();		
		void old_set_BallSizeType(int m_BallSizeType);
		int old_get_BallSizeType();
	private:
		int old_BallSize_toIdx(int BallSize);
	};

}

#endif