#ifndef __ObjectManager_h__
#define __ObjectManager_h__

#include <Windows.h>
#include <string>
#include <vector>
#include <tchar.h>

using std::vector;
typedef std::basic_string<TCHAR> tstring;

namespace RollingBall
{
	class ObjectInfo {
	public:
		tstring name;
		BOOL has_mask;
		struct _texture {
			vector<tstring> name;
			vector<int> size;
		} texture;

		//텍스쳐 개수를 반환
		int count_texture();
		//텍스쳐 사이즈 개수를 반환
		int count_texture_size();
	};

	class Object {
	public:
		ObjectInfo info;
	private:

	};

	class ObjectManager 
	{
	private:
		static BOOL flag_isObjectInfoInit;
	public:
		static vector<ObjectInfo> object_info;

		BOOL init(HWND hwnd);
		BOOL init_object_info(HWND hwnd);
		BOOL isInitObjectInfo();

	};
}

#endif