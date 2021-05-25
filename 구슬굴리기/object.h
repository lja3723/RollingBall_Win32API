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
		void name(tstring name);

		BOOL has_mask();
		void has_mask(BOOL has_mask);

		LPCTSTR texture_name(int idx);
		void texture_name(int idx, tstring name);
		void texture_name_resize(int size);
		void texture_name_push_back(tstring name);

		int texture_size(int idx);
		void texture_size(int idx, int size);
		void texture_size_resize(int size);
		void texture_size_push_back(int size);

		//텍스쳐 개수를 반환
		int count_texture();
		//텍스쳐 사이즈 개수를 반환
		int count_texture_size();
		//모든 정보를 초기화
		void clear();
	};

	class ObjectBitmapInfoVector
	{

	private:
		static BOOL flag_isInit_bitmap_info;
		static vector<ObjectBitmapInfo> _bitmap_info;

	public:
		BOOL load(HWND hwnd, LPCTSTR filename);
		BOOL isLoaded();
		ObjectBitmapInfo& get_bmpInfo(int idx);
		int index(LPCTSTR object_name);
		int count_object();
	};


	class Object 
	{
	private:
		struct _idx {
			int object;
			int texture;
		} idx;

		static ObjectBitmapInfoVector _bmpInfoVec;

	protected:

	public:
		static ObjectBitmapInfo bmpInfo;
		PhysicalValue physical;

		//BOOL init(HWND hwnd);
		BOOL init();
	};


	class Ball : Object 
	{
	private:
	public:

	};

	class Background : Object 
	{
	private:
	public:

	};
}

#endif