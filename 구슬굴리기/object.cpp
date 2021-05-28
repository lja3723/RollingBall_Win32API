#include "object.h"
#include "file.h"
#include "debugger.h"

using namespace RollingBall;

/*********************************************** 
************************************************ 
*** 
***		ObjectBitmapInfo class
***
************************************************  
************************************************/
BOOL ObjectBitmapInfo::isIdxInRange(int idx, int rangeMax)
{
	return 0 <= idx && idx < rangeMax;
}

LPCTSTR ObjectBitmapInfo::name()
{
	return _name.c_str();
}
void ObjectBitmapInfo::name(tstring name)
{
	_name = name;
}

BOOL ObjectBitmapInfo::has_mask()
{
	return _has_mask;
}
void ObjectBitmapInfo::has_mask(BOOL has_mask)
{
	_has_mask = has_mask;
}

LPCTSTR ObjectBitmapInfo::texture_name(int idx)
{
	if (!isIdxInRange(idx, (int)_texture._name.size())) return _T("");
	return _texture._name[idx].c_str();
}
void ObjectBitmapInfo::texture_name(int idx, tstring name)
{
	if (!isIdxInRange(idx, (int)_texture._name.size())) return;
	_texture._name[idx] = name;
}
void ObjectBitmapInfo::texture_name_resize(int size)
{
	if (size < 0) size = 0;
	_texture._name.resize(size);
}
void ObjectBitmapInfo::texture_name_push_back(tstring name)
{
	_texture._name.push_back(name);
}

pixel ObjectBitmapInfo::texture_size(int idx)
{
	if (!isIdxInRange(idx, (int)_texture._size.size())) return 0;
	return _texture._size[idx];
}
void ObjectBitmapInfo::texture_size(int idx, pixel size)
{
	if (!isIdxInRange(idx, (int)_texture._size.size())) return;
	_texture._size[idx] = size;
}
void ObjectBitmapInfo::texture_size_resize(pixel size)
{
	if (size < 0) size = 0;
	_texture._size.resize(size);
}
void ObjectBitmapInfo::texture_size_push_back(pixel size)
{
	_texture._size.push_back(size);
}

int ObjectBitmapInfo::count_texture()
{
	return (int)_texture._name.size();
}
int ObjectBitmapInfo::count_texture_size()
{
	return (int)_texture._size.size();
}

void ObjectBitmapInfo::clear()
{
	_name = _T("");
	_has_mask = FALSE;
	_texture._name.clear();
	_texture._size.clear();
}



/***********************************************
************************************************
***
***		ObjectBitmapInfoVector class
***
************************************************
************************************************/
BOOL ObjectBitmapInfoVector::flag_isLoaded = FALSE;
vector<ObjectBitmapInfo> ObjectBitmapInfoVector::_bitmap_info = vector<ObjectBitmapInfo>();
ObjectBitmapInfo ObjectBitmapInfoVector::_dummy_bmpInfo = ObjectBitmapInfo();


BOOL RollingBall::ObjectBitmapInfoVector::isIdxInRange(int idx, int idxMax)
{
	return 0 <= idx && idx < idxMax;
}

BOOL ObjectBitmapInfoVector::Load(HWND hwnd, LPCTSTR filename)
{
	File file;
	const int errmsg_len = 256;
	TCHAR errmsg[errmsg_len];
	LPCTSTR open_failed = _T("%s 파일을 열 수 없습니다.");
	LPCTSTR format_invalid = _T("%s 파일의 양식이 잘못되었습니다.");


	if (isLoaded()) return TRUE;

	//dummy data 초기화
	_dummy_bmpInfo.name(_T("__dummy__"));
	_dummy_bmpInfo.has_mask(FALSE);
	_dummy_bmpInfo.texture_name_push_back(_T("__dummy__"));
	_dummy_bmpInfo.texture_size_push_back(0);

	//파일을 열 수 없음
	if (!file.open(filename, _T("r")))
	{
		_stprintf_s(errmsg, errmsg_len, open_failed, filename);
		MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
		return FALSE;
	}

	//파일을 한 줄씩 읽어 object에 저장한다.
	TCHAR line[128];
	ObjectBitmapInfo bmpInfo;
	BOOL isObjectReading = FALSE;
	//int objidx = -1;
	while (file.readLine(line, sizeof(line)))
	{

		//공백 라인이거나 주석일 경우 패스
		if (_tcslen(line) == 0) continue;
		if (line[0] == _T('#')) continue;


		//오브젝트 정보를 읽는 중이 아닐 때...
		if (!isObjectReading) {
			//파일이 시작됨
			if (_tcscmp(line, _T("<object_bmp_info.txt>")) == 0)
			{
				bmpInfo.clear();
				_bitmap_info.clear();
				continue;
			}

			//오브젝트 정보를 읽기 시작함
			else if (_tcscmp(line, _T("<object>")) == 0) {
				isObjectReading = TRUE;
				continue;
			}

			//파일이 종료됨
			//마지막 인덱스에 더미 데이터 저장
			else if (_tcscmp(line, _T("</object_bmp_info.txt>")) == 0)
			{
				break;
			}

			//파일 양식이 잘못됨
			else {
				_stprintf_s(errmsg, errmsg_len, format_invalid, filename);
				MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
				file.close();
				return FALSE;
			}
		}

		//오브젝트 정보를 읽는 중일 때..
		else
		{
			//오브젝트의 이름을 저장
			if (_tcscmp(line, _T("name=")) == 0) {
				file.readLine(line, sizeof(line));
				bmpInfo.name(line);
				continue;
			}

			//오브젝트의 마스크 여부 저장
			else if (_tcscmp(line, _T("has_mask=")) == 0) {
				file.readLine(line, sizeof(line));
				BOOL flag = FALSE;
				if (_tcscmp(line, _T("TRUE")) == 0) flag = TRUE;
				else if (_tcscmp(line, _T("FALSE")) == 0) flag = FALSE;
				else {
					//파일 양식이 잘못됨
					_stprintf_s(errmsg, errmsg_len, format_invalid, filename);
					MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
					file.close();
					return FALSE;
				}

				bmpInfo.has_mask(flag);
				continue;
			}

			//오브젝트 텍스쳐 이름 저장
			else if (_tcscmp(line, _T("<texture_name>")) == 0) {
				while (TRUE)
				{
					if (!file.readLine(line, sizeof(line))) {
						//파일 양식이 잘못됨
						_stprintf_s(errmsg, errmsg_len, format_invalid, filename);
						MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
						file.close();
						return FALSE;
					}
					if (_tcscmp(line, _T("</texture_name>")) == 0) break;
					bmpInfo.texture_name_push_back(line);
				}
				continue;
			}

			//오브젝트 텍스쳐 크기 값 저장
			else if (_tcscmp(line, _T("<texture_size_value>")) == 0) {
				while (TRUE)
				{
					if (!file.readLine(line, sizeof(line))) {
						//파일 양식이 잘못됨
						_stprintf_s(errmsg, errmsg_len, format_invalid, filename);
						MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
						file.close();
						return FALSE;
					}
					if (_tcscmp(line, _T("</texture_size_value>")) == 0) break;
					bmpInfo.texture_size_push_back(_ttoi(line));
				}
				continue;
			}

			//오브젝트 정보를 모두 읽었음
			else if (_tcscmp(line, _T("</object>")) == 0) {
				_bitmap_info.push_back(bmpInfo);
				bmpInfo.clear();
				isObjectReading = FALSE;
				continue;
			}

			//파일 양식이 잘못됨
			else {
				_stprintf_s(errmsg, errmsg_len, format_invalid, filename);
				MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
				file.close();
				return FALSE;
			}
		}
	}

	//파일을 닫는다.
	file.close();
	flag_isLoaded = TRUE;

#ifdef __debugger_h__
	debuggerMessage("This massage showed by 'object.cpp' by ObjectBitmapInfoVector::Load()");
	for (int i = 0; i < count_object(); i++)
	{
		tstring name, value;
		for (int j = 0; j < _bitmap_info[i].count_texture(); j++)
			name += _bitmap_info[i].texture_name(j), name += _T(" ");
		for (int j = 0; j < _bitmap_info[i].count_texture_size(); j++)
			value += std::to_wstring(_bitmap_info[i].texture_size(j)), value += _T(" ");

		debuggerMessage(
			"object index: %d\n"
			"object name: %s\n\n"

			"texture count: %d\n"
			"texture name: %s\n\n"

			"texture value count: %d\n"
			"values: %s\n\n"

			"has mask: %s",
			i, _bitmap_info[i].name(),
			(int)_bitmap_info[i].count_texture(), name.c_str(),
			(int)_bitmap_info[i].count_texture_size(), value.c_str(),
			(_bitmap_info[i].has_mask() ? _T("TRUE") : _T("FALSE"))
		);
	}
#endif
	return TRUE;
}
BOOL ObjectBitmapInfoVector::isLoaded()
{
	return flag_isLoaded;
}
ObjectBitmapInfo& ObjectBitmapInfoVector::get_bmpInfo(int idx_object)
{
	if (!isLoaded()) return _dummy_bmpInfo;
	if (!isIdxInRange(idx_object, count_object()))
		return _dummy_bmpInfo;
	return _bitmap_info[idx_object];
}
int RollingBall::ObjectBitmapInfoVector::index(LPCTSTR object_name)
{
	for (int i = 0; i < count_object(); i++)
		if (_tcscmp(object_name, _bitmap_info[i].name()) == 0)
			return i;
	return -1;
}

int ObjectBitmapInfoVector::count_object()
{
	return (int)_bitmap_info.size();
}
int RollingBall::ObjectBitmapInfoVector::count_bitmap(int idx_object)
{
	if (!isIdxInRange(idx_object, count_object())) return 0;
	return (int)_bitmap_info[idx_object].count_texture()
		* (int)_bitmap_info[idx_object].count_texture_size()
		* (_bitmap_info[idx_object].has_mask() ? 2 : 1);
}
int RollingBall::ObjectBitmapInfoVector::count_bitmap_files()
{
	int bitmap_files = 0;
	for (int i = 0; i < count_object(); i++)
		bitmap_files += count_bitmap(i);
	return bitmap_files;
}



/***********************************************
************************************************
***
***		Object class (abstract)
***
************************************************
************************************************/

void RollingBall::Object::init(LPCTSTR object_name)
{
	idx.object = ObjectBitmapInfoVector::index(object_name);
	bmpInfo = ObjectBitmapInfoVector::get_bmpInfo(idx.object);
}

Object::Object() 
{ 
	_name = _T(""); 
	physical.size = 1;
	physical.mass = 1;
	physical.rotate_angle = 0;
	physical.pos(0, 0);
	physical.speed(0, 0);
	physical.accel(0, 0);
}

int RollingBall::Object::count_texture()
{
	return bmpInfo.count_texture();
}
int RollingBall::Object::count_texture_size()
{
	return bmpInfo.count_texture_size();
}
BOOL RollingBall::Object::has_mask()
{
	return bmpInfo.has_mask();
}
LPCTSTR RollingBall::Object::bitmap_name()
{
	return bmpInfo.name();
}

void RollingBall::Object::name(LPCTSTR name)
{
	_name = name;
}
LPCTSTR RollingBall::Object::name()
{
	return _name.c_str();
}

LPCTSTR RollingBall::Object::texture()
{
	return bmpInfo.texture_name(idx.texture);
}
void RollingBall::Object::texture(LPCTSTR texture_name)
{
	for (int i = 0; i < bmpInfo.count_texture(); i++)
		if (_tcscmp(texture_name, bmpInfo.texture_name(i)) == 0)
		{
			idx.texture = i;
			return;
		}
	idx.texture = 0;
}
pixel RollingBall::Object::texture_size(Scaler& scale)
{
	return bmpInfo.texture_size(index_texture_size(scale));
}

int RollingBall::Object::index_object()
{
	return idx.object;
}
int RollingBall::Object::index_texture()
{
	return idx.texture;
}
int RollingBall::Object::index_texture_size(Scaler& scale)
{
	int i;
	for (i = 0; i < count_texture_size(); i++)
		if (scale.px(physical.size) <= bmpInfo.texture_size(i))
			return i;
	return i - 1;	//가장 큰 텍스쳐 크기의 인덱스를 리턴한다
}


/***********************************************
************************************************
***
***		Ball class
***
************************************************
************************************************/
RollingBall::Ball::Ball(LPCTSTR texture_name)
{
	init(_T("ball"));
	texture(texture_name);
}



/***********************************************
************************************************
***
***		Background class
***
************************************************
************************************************/
RollingBall::Background::Background(LPCTSTR texture_name)
{
	init(_T("floor"));
	texture(texture_name);
}
