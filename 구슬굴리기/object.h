#pragma once
#ifndef __object_h__
#define __object_h__

#include <Windows.h>
#include <string>
#include <vector>
#include <tchar.h>
#include "physics.h"
#include "scaler.h"

using std::vector;
typedef std::basic_string<TCHAR> tstring;

namespace RollingBall
{
	class ObjectBitmapInfo {
	private:
		tstring _name;
		BOOL _has_mask;
		struct __texture {
			vector<tstring> _name;
			vector<int> _size;
		} _texture;

		BOOL isIdxInRange(int idx, int rangeMax);

	public:
		LPCTSTR name();
		void name_set(tstring name);

		BOOL has_mask();
		void has_mask_set(BOOL has_mask);

		LPCTSTR texture_name(int idx);
		void texture_name_resize(int size);
		void texture_name_set(int idx, tstring name);
		void texture_name_push_back(tstring name);

		int texture_size(int idx);
		void texture_size_resize(int size);
		void texture_size_set(int idx, int size);
		void texture_size_push_back(int size);

		//텍스쳐 개수를 반환
		int count_texture();
		//텍스쳐 사이즈 개수를 반환
		int count_texture_size();
		//모든 정보를 초기화
		void clear();
	};


	class Object 
	{
	private:
		static BOOL flag_isInitVectorObjectBitmapInfo;
		static vector<ObjectBitmapInfo> _vector_object_bitmap_info;
		ObjectBitmapInfo& get_bmpInfo(int idx);
		BOOL init_vector_object_info(HWND hwnd);
		BOOL init(HWND hwnd);
		BOOL isInitVectorObjectBitmapInfo();

		struct _idx {
			int object;
			int texture;
		};

	public:
		static ObjectBitmapInfo bmpInfo;
		int object_count();
		BOOL isInitBmpInfo();
		PhysicalValue physical;
	};


	class Ball : Object 
	{
	private:
	public:

	};

	class Floor : Object 
	{
	private:
	public:

	};
}

#endif