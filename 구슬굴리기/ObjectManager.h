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
	private:
		tstring _name;
		BOOL _has_mask;
		struct __texture {
			vector<tstring> _name;
			vector<int> _size;
		} _texture;

	public:
		void name_set(tstring m_name);
		LPCTSTR name();

		void has_mask_set(BOOL m_has_mask);
		BOOL has_mask();

		LPCTSTR texture_name(int idx);
		void texture_name_push_back(tstring m_texture);
		void texture_name_resize(int size);
		void texture_name_clear();

		int texture_size(int idx);
		void texture_size_push_back(int m_size);
		void texture_size_resize(int size);
		void texture_size_clear();

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

}

#endif