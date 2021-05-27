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
		//이름을 반환한다
		LPCTSTR name();

		//이름을 설정한다
		void name(tstring name);

		//마스크 여부를 반환한다
		BOOL has_mask();

		//마스크 여부를 설정한다
		void has_mask(BOOL has_mask);

		//idx번째 텍스쳐 이름을 반환한다
		LPCTSTR texture_name(int idx);

		//idx번째 텍스쳐를 설정한다
		void texture_name(int idx, tstring name);
		void texture_name_resize(int size);
		void texture_name_push_back(tstring name);

		//idx번째 텍스쳐 사이즈를 반환한다
		pixel texture_size(int idx);

		//idx번째 텍스쳐 사이즈를 설정한다
		void texture_size(int idx, pixel size);
		void texture_size_resize(pixel size);
		void texture_size_push_back(pixel size);

		//텍스쳐 개수를 반환한다
		int count_texture();

		//텍스쳐 사이즈 개수를 반환한다
		int count_texture_size();

		//모든 정보를 초기화
		void clear();
	};


	class ObjectBitmapInfoVector
	{

	private:
		// _bitmap_info는 object_bitmap_info.txt 파일의 정보로 초기화된다
		// _bitmap_info는 프로그램 실행 중 한 번만 초기화된다
		static BOOL flag_isLoaded;
		static vector<ObjectBitmapInfo> _bitmap_info;

		// 사용자가 잘못된 조작을 할 경우 반환될수 있는 더미데이터
		// 역시 한번만 초기화된다
		static ObjectBitmapInfo _dummy_bmpInfo;
		BOOL isIdxInRange(int idx, int idxMax);

	public:
		//프로그램 실행 중 한 번은 실행되어야 한다
		BOOL Load(HWND hwnd, LPCTSTR filename);
		BOOL isLoaded();
		ObjectBitmapInfo& get_bmpInfo(int idx_object);
		int index(LPCTSTR object_name);

		//오브젝트의 개수를 반환한다
		int count_object();

		//오브젝트와 관련된 비트맵 개수를 반환한다
		int count_bitmap(int idx_object);

		//비트맵 파일 총 개수를 반환한다
		int count_bitmap_files();
	};

	
	class Object abstract
	{
	/*****************************
	*	protected variables
	******************************/
	protected:
		struct _idx {
			int object;
			int texture;
		} idx;
		static ObjectBitmapInfoVector _bmpInfoVec;
		ObjectBitmapInfo bmpInfo;
		tstring _name;



	/*****************************
	*	protected functions
	******************************/
	protected:
		//상속받은 클래스는 생성자에서 반드시 호출해야 한다
		void init(LPCTSTR object_name);


	/*****************************
	*	pubilc variables
	******************************/
	public:
		//물리값
		PhysicalValue physical;



	/*****************************
	*	pubilc functions
	******************************/
	public:
		Object();

		//오브젝트의 텍스쳐 개수를 반환한다
		int count_texture();
		//오브젝트의 텍스쳐 사이즈 수를 반환한다
		int count_texture_size();
		//오브젝트가 마스크 이미지를 가지는지 반환한다
		BOOL has_mask();
		//오브젝트 비트맵 이름을 반환한다
		LPCTSTR bitmap_name();

		//오브젝트의 이름을 설정한다
		void name(LPCTSTR name);
		//오브젝트의 이름을 반환한다
		LPCTSTR name();

		//오브젝트의 인덱스를 반환한다
		int index_object();
		//오브젝트의 텍스쳐 인덱스를 반환한다
		int index_texture();

		//scaler를 기반으로 오브젝트 size를 가장 잘 표현하는 텍스쳐 사이즈의 인덱스를 반환한다
		int index_texture_size(Scaler& scale);

		//오브젝트의 텍스쳐 이름을 반환한다
		LPCTSTR texture();
		//오브젝트의 텍스쳐를 설정한다
		void texture(LPCTSTR texture_name);

		//texture_size를 오브젝트가 가진 텍스쳐 중 가장 근접한 크기로 어림한다
		//pixel round_texture_size(pixel texture_size);

		//scaler를 기반으로 오브젝트 size를 가장 잘 표현하는 texture size를 반환한다
		pixel texture_size(Scaler& scale);
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