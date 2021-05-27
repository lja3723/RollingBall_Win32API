#pragma once
#ifndef __bitmap_h__
#define __bitmap_h__

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "object.h"
#include "scaler.h"

/*
* 
* Bitmap:
* 프로그램에서 사용하는 비트맵을 관리하는 클래스를 정의
* 
*/

namespace RollingBall
{
	class Bitmap
	{
	private:
		/*******************************
		*	private variables: static
		********************************/
		static HINSTANCE hInstance;

		//BMPFILEMACRO의 실질적 길이는 _file_count임
		//새 비트맵 리소스를 추가시 수정
		//수정 시 object_bmp_info.txt도 수정 필요
		//매크로 작성 순서는 object_bmp_info.txt와 일치시켜야 함
		//
		const UINT BMPFILEMACRO[256] = {
			IDB_BALL_IRON1_0032, IDB_BALL_IRON1_0064, IDB_BALL_IRON1_0128, IDB_BALL_IRON1_0256,
			IDB_BALL_IRON1_0032M, IDB_BALL_IRON1_0064M, IDB_BALL_IRON1_0128M, IDB_BALL_IRON1_0256M,

			IDB_BALL_IRON2_0032, IDB_BALL_IRON2_0064, IDB_BALL_IRON2_0128, IDB_BALL_IRON2_0256,
			IDB_BALL_IRON2_0032M, IDB_BALL_IRON2_0064M, IDB_BALL_IRON2_0128M, IDB_BALL_IRON2_0256M,

			IDB_FLOOR_WOOD1_0256, IDB_FLOOR_WOOD1_0512, IDB_FLOOR_WOOD1_1024
		};

		//비트맵 파일 개수: init 함수 호출시 자동으로 초기화됨
		static int _file_count;

		//로드된 hBitmap들을 담을 벡터
		static vector<HBITMAP> hBitmap;
		


		/************************************
		*	private functions
		*************************************/
		//bitmap_file_count를 초기화하는 함수(init시 수행)
		void init_file_count();

		//hBitmap 벡터에 hBitmap들을 로드하는 함수(init시 수행)
		void init_hBitmap();

		//hBitmap 벡터에 저장된 hBitmap들을 삭제하는 함수(소멸자 호출시 수행)
		void delete_hBitmap();

	


		/************************************
		*	private functions
		*   BOOL returns
		*************************************/
		BOOL isInitFileCount();
		BOOL isInitHBitmap();

	public:
		//Bitmap 클래스 변수를 사용하기 전 반드시 수행해야 한다
		BOOL init(HINSTANCE m_hInstance);
		BOOL isInit();
		~Bitmap();

		HBITMAP get(int index);
		//HBITMAP get(Object& object, pixel texture_size, BOOL mask_texture = FALSE);
		HBITMAP get(Object& object, Scaler& scale, BOOL mask_texture = FALSE);
		
		HBITMAP operator()(int index);
		//HBITMAP operator()(Object& object, pixel texture_size, BOOL mask_texture = FALSE);
		HBITMAP operator()(Object& object, Scaler& scale, BOOL mask_texture = FALSE);

		//int idx(Object& object, pixel texture_size, BOOL mask_texture = FALSE);
		int idx(Object& object, Scaler& scale, BOOL mask_texture = FALSE);

		int file_count();


		//hDC와 호환되는 크기가 rt인 hBitmap을 생성한다
		HBITMAP create_hDC_compatible(HDC hdc, RECT& rt);

		//hDC 호환 hBitmap을 삭제한다
		void delete_hDC_compatible(HBITMAP hDCcompatibleBitmap);
	};

}

#endif