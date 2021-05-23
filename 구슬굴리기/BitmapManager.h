#pragma once
#ifndef __BitmapManager_h__
#define __BitmapManager_h__

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
		HWND hwnd;

		struct BitmapManagerObject {
			tstring name;
			BOOL has_mask;
			struct {
				vector<tstring> name;
				vector<int> value;
			} texture;
		};

		struct {
			int object;
			int texture;
			int texture_size;
			BOOL mask;
		} curselidx;



		/*******************************
		*	private variables: static
		********************************/
		//BMPFILEMACRO의 실질적 길이는 bitmap_file_count임
		//새 비트맵 리소스를 추가시 수정
		//수정 시 object_info.txt도 수정 필요
		const UINT BMPFILEMACRO[256] = {
			IDB_BALL_IRON1_032, IDB_BALL_IRON1_064, IDB_BALL_IRON1_128, IDB_BALL_IRON1_256,
			IDB_BALL_IRON1_032M, IDB_BALL_IRON1_064M, IDB_BALL_IRON1_128M, IDB_BALL_IRON1_256M,
			IDB_FLOOR_WOOD1_256
		};


		//오브젝트에 대한 정보를 담은 벡터
		static vector<BitmapManagerObject> object_info;
		//비트맵 파일 개수: init 함수 호출시 자동으로 초기화됨
		static int bitmap_file_count;
		//로드된 hBitmap들을 담을 벡터
		static vector<HBITMAP> hBitmap;
		static HINSTANCE hInstance;
		


		/************************************
		*	private functions
		*************************************/
		//파일에서 한 라인씩 읽어서 line에 저장해주는 함수
		BOOL ReadLine(HANDLE hFile, LPTSTR line, int lineLength);
		//object vector를 object_info.txt 기반으로 초기화하는 함수
		BOOL init_object_info(HWND hwnd);
		//bitmap_file_count를 초기화하는 함수

		void init_bitmap_file_count();
		//hBitmap 벡터에 hBitmap들을 로드하는 함수
		void init_hBitmap();
		//hBitmap 벡터에 저장된 hBitmap들을 삭제하는 함수
		void delete_hBitmap();
		//curselidx 요소들을 초기화하는 함수
		void init_curselidx();
		//obj의 총 파일 개수를 구하는 함수
		int get_file_count(int objidx);



		/************************************
		*	private functions
		*   BOOL returns
		*************************************/
		BOOL isInitObjectInfo();
		BOOL isInitBitmapFileCount();
		BOOL isInitHBitmap();
		//idx의 범위가 정상적이도록 재배열한다
		void arrange_idx(int& objidx, int& textureidx, int& sizeidx, BOOL& mask);

	public:
		//BitmapManager 클래스 변수를 사용하기 전 반드시 수행해야 한다
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd);
		BOOL isInit();
		~BitmapManager();

		HBITMAP get(int index);
		HBITMAP create_hDC_compatible(HDC hdc, RECT& rt);
		void delete_hDC_compatible(HBITMAP hDCcompatibleBitmap);

		
		int get_curr_sel_idx();
		int get_curr_object_idx();
		LPCTSTR get_curr_object_name();

		int get_curr_texture_idx();
		LPCTSTR get_curr_texture_name();

		int get_curr_texture_size_idx();
		int get_curr_texture_size();

		BOOL get_curr_object_has_mask();

		int get_bitmap_file_count();


		int index(int objidx = 0, int textureidx = 0, int sizeidx = 0, BOOL m_mask = FALSE);
		int index(LPCTSTR m_obj, LPCTSTR m_texture, int m_size, BOOL m_mask);

		int object(LPCTSTR m_obj);
		LPCTSTR object(int m_objidx);

		int texture(LPCTSTR m_texture);
		LPCTSTR texture(int m_textureidx);

		int size(int m_size);
		int idx_to_size(int sizeidx);

		void set_cur_sel(int objidx = 0, int textureidx = 0, int sizeidx = 0, BOOL m_mask = FALSE);
		void set_cur_sel(LPCTSTR m_obj, LPCTSTR m_texture, int m_size, BOOL m_mask);

		//get() 함수를 호출한다
		HBITMAP operator[](int index);
	};

}

#endif