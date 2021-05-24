#pragma once
#ifndef __object_h__
#define __object_h__

#include <Windows.h>
#include <string>
#include <vector>
#include <tchar.h>

using std::vector;
typedef std::basic_string<TCHAR> tstring;

namespace RollingBall
{
	class ObjectInfo {
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


	class ObjectManager 
	{
	private:
		static BOOL flag_isObjectInfoInit;
		static vector<ObjectInfo> _object_info;
	public:

		ObjectInfo& object_info(int idx);
		int object_count();
		BOOL init(HWND hwnd);
		BOOL init_object_info(HWND hwnd);
		BOOL isInitObjectInfo();
	};



	class Object {
	private:

	public:
		static ObjectInfo info;
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