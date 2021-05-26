#include "object.h"
#include "file.h"

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
	if (!isIdxInRange(idx, (int)_texture._name.size())) return _T("idx error");
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

int ObjectBitmapInfo::texture_size(int idx)
{
	if (!isIdxInRange(idx, (int)_texture._size.size())) return 0;
	return _texture._size[idx];
}
void ObjectBitmapInfo::texture_size(int idx, int size)
{
	if (!isIdxInRange(idx, (int)_texture._size.size())) return;
	_texture._size[idx] = size;
}
void ObjectBitmapInfo::texture_size_resize(int size)
{
	if (size < 0) size = 0;
	_texture._size.resize(size);
}
void ObjectBitmapInfo::texture_size_push_back(int size)
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
***		Object class
***
************************************************
************************************************/

vector<ObjectBitmapInfo> ObjectBitmapInfoVector::_bitmap_info = vector<ObjectBitmapInfo>();
BOOL ObjectBitmapInfoVector::flag_isInit_bitmap_info = FALSE;

ObjectBitmapInfo& ObjectBitmapInfoVector::get_bmpInfo(int idx)
{
	if (!(0 <= idx && idx < object_count())) 
		idx = object_count(); //idx를 더미 데이터로 설정한다
	return _bitmap_info[idx];
}
int RollingBall::ObjectBitmapInfoVector::index(LPCTSTR object_name)
{
	return 0;
}
BOOL ObjectBitmapInfoVector::load(HWND hwnd, LPCTSTR filename)
{
	//LPCTSTR filename = _T("..\\res\\bmp\\object_info.txt");
	File file;

	if (!file.open(filename, _T("r")))
	{
		TCHAR errmsg[256];
		_stprintf_s(errmsg, 256, _T("%s 파일을 열 수 없습니다."), filename);
		MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
		return FALSE;
	}


	//파일을 한 줄씩 읽어 object에 저장한다.
	TCHAR line[128];
	ObjectBitmapInfo oi;
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
			if (_tcscmp(line, _T("<object_info.txt>")) == 0)
			{
				oi.clear();
				_bitmap_info.clear();
				continue;
			}

			//오브젝트 정보를 읽기 시작함
			if (_tcscmp(line, _T("<object>")) == 0) {
				isObjectReading = TRUE;
				continue;
			}

			//파일이 종료됨
			//마지막 인덱스에 더미 데이터 저장
			if (_tcscmp(line, _T("</object_info.txt>")) == 0)
			{
				oi.name(_T("NULL"));
				oi.has_mask(FALSE);
				oi.texture_name_push_back(_T("NULL"));
				oi.texture_size_push_back(0);
				_bitmap_info.push_back(oi);
				break;
			}
		}
		//오브젝트 정보를 읽는 중일 때..
		else
		{
			//오브젝트의 이름을 저장
			if (_tcscmp(line, _T("name=")) == 0) {
				file.readLine(line, sizeof(line));
				oi.name(line);
				continue;
			}

			//오브젝트의 마스크 여부 저장
			if (_tcscmp(line, _T("has_mask=")) == 0) {
				file.readLine(line, sizeof(line));
				BOOL flag = FALSE;
				if (_tcscmp(line, _T("TRUE")) == 0) flag = TRUE;
				if (_tcscmp(line, _T("FALSE")) == 0) flag = FALSE;
				oi.has_mask(flag);
				continue;
			}

			//오브젝트 텍스쳐 이름 저장
			if (_tcscmp(line, _T("<texture_name>")) == 0) {
				while (TRUE)
				{
					file.readLine(line, sizeof(line));
					if (_tcscmp(line, _T("</texture_name>")) == 0) break;
					oi.texture_name_push_back(line);
				}
				continue;
			}

			//오브젝트 텍스쳐 크기 값 저장
			if (_tcscmp(line, _T("<texture_size_value>")) == 0) {
				while (TRUE)
				{
					file.readLine(line, sizeof(line));
					if (_tcscmp(line, _T("</texture_size_value>")) == 0) break;
					oi.texture_size_push_back(_ttoi(line));
				}
				continue;
			}

			//오브젝트 정보를 모두 읽었음
			if (_tcscmp(line, _T("</object>")) == 0) {
				_bitmap_info.push_back(oi);
				oi.clear();
				isObjectReading = FALSE;
				continue;
			}
		}
	}

	//파일을 닫는다.
	file.close();
	return TRUE;
}
BOOL ObjectBitmapInfoVector::isLoaded()
{
	return flag_isInit_bitmap_info;
}




int Object::object_count()
{
	//_vector_object_bitmap_info의 마지막 원소는 더미 데이터다
	return (int)_bitmap_info.size() - 1;
}
BOOL Object::init()
{
	if (_bmpInfoVec.isLoaded()) return TRUE;
	bmpInfo = _bmpInfoVec.get_bmpInfo(0);


	return TRUE;
}
void RollingBall::Object::init()
{
	return;
}
