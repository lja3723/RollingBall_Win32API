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
			vector<pixel> _size;
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

		pixel texture_size(int idx);
		void texture_size(int idx, pixel size);
		void texture_size_resize(pixel size);
		void texture_size_push_back(pixel size);

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
		static BOOL flag_isLoaded;
		static vector<ObjectBitmapInfo> _bitmap_info;
		static ObjectBitmapInfo _dummy_bmpInfo;
		BOOL isIdxInRange(int idx, int idxMax);

	public:
		BOOL Load(HWND hwnd, LPCTSTR filename);
		BOOL isLoaded();
		ObjectBitmapInfo& get_bmpInfo(int idx_object);
		int index(LPCTSTR object_name);

		int count_object();
		int count_bitmap(int idx_object);
		int count_bitmap_files();
	};

	class Object 
	{
	private:
		struct _idx {
			int object;
			int texture;
		} idx;

	class Object abstract
	{
	protected:
		struct _idx {
			int object;
			int texture;
		} idx;
		static ObjectBitmapInfoVector _bmpInfoVec;
		ObjectBitmapInfo bmpInfo;

		void init(LPCTSTR object_name);

	public:
		PhysicalValue physical;
		int count_texture();
		int count_texture_size();
		BOOL has_mask();
		LPCTSTR name();

		int index_object();
		int index_texture();
		int index_texture(pixel texture_size);
		LPCTSTR texture();
		void texture(LPCTSTR texture_name);
	};




	class Ball : public Object 
	{
	private:
	public:
		Ball(LPCTSTR texture_name = _T("iron1"));
	};

	class Background : public Object
	{
	private:
	public:
		Background(LPCTSTR texture_name = _T("wood1"));

	};
}

#endif