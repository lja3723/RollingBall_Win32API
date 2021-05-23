#include "ObjectManager.h"

using namespace RollingBall;

void ObjectInfo::name_set(tstring m_name)
{
	_name = m_name;
}
LPCTSTR ObjectInfo::name()
{
	return _name.c_str();
}


void ObjectInfo::has_mask_set(BOOL m_has_mask)
{
	_has_mask = m_has_mask;
}
BOOL ObjectInfo::has_mask()
{
	return _has_mask;
}


LPCTSTR ObjectInfo::texture_name(int idx)
{
	if (0 <= idx && idx <= count_texture())
		return _texture._name[idx].c_str();
	else
		return NULL;
}
void ObjectInfo::texture_name_push_back(tstring m_texture)
{
	_texture._name.push_back(m_texture);
}
void ObjectInfo::texture_name_resize(int size)
{
	_texture._name.resize(size);
}
void ObjectInfo::texture_name_clear()
{
	_texture._name.clear();
}


int ObjectInfo::texture_size(int idx)
{
	if (0 <= idx && idx <= count_texture_size())
		return _texture._size[idx];
	else
		return 0;
}
void ObjectInfo::texture_size_push_back(int m_size)
{
	_texture._size.push_back(m_size);
}
void ObjectInfo::texture_size_resize(int size)
{
	_texture._size.resize(size);
}
void ObjectInfo::texture_size_clear()
{
	_texture._size.clear();
}


int ObjectInfo::count_texture()
{
	return _texture._name.size();
}
int ObjectInfo::count_texture_size()
{
	return _texture._size.size();
}