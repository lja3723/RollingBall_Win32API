#include "ObjectManager.h"

using namespace RollingBall;

int ObjectInfo::count_texture()
{
	return texture.name.size();
}
int ObjectInfo::count_texture_size()
{
	return texture.size.size();
}