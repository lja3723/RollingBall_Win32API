#include "ObjectManager.h"

using namespace RollingBall;

void RollingBall::ObjectInfo::name_set(tstring m_name)
{
	_name = m_name;
}
LPCTSTR RollingBall::ObjectInfo::name()
{
	return _name.c_str();
}


void RollingBall::ObjectInfo::has_mask_set(BOOL m_has_mask)
{
	_has_mask = m_has_mask;
}
BOOL RollingBall::ObjectInfo::has_mask()
{
	return _has_mask;
}


LPCTSTR RollingBall::ObjectInfo::texture_name(int idx)
{
	if (0 <= idx && idx <= count_texture())
		return _texture._name[idx].c_str();
	else
		return NULL;
}
void RollingBall::ObjectInfo::texture_name_push_back(tstring m_texture)
{
	_texture._name.push_back(m_texture);
}
void RollingBall::ObjectInfo::texture_name_resize(int size)
{
	_texture._name.resize(size);
}
void RollingBall::ObjectInfo::texture_name_clear()
{
	_texture._name.clear();
}


int RollingBall::ObjectInfo::texture_size(int idx)
{
	if (0 <= idx && idx <= count_texture_size())
		return _texture._size[idx];
	else
		return 0;
}
void RollingBall::ObjectInfo::texture_size_push_back(int m_size)
{
	_texture._size.push_back(m_size);
}
void RollingBall::ObjectInfo::texture_size_resize(int size)
{
	_texture._size.resize(size);
}
void RollingBall::ObjectInfo::texture_size_clear()
{
	_texture._size.clear();
}


int RollingBall::ObjectInfo::count_texture()
{
	return _texture._name.size();
}
int RollingBall::ObjectInfo::count_texture_size()
{
	return _texture._size.size();
}