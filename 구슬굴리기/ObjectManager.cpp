#include "ObjectManager.h"
#include "FileManager.h"

using namespace RollingBall;

vector<ObjectInfo> ObjectManager::object_info = vector<ObjectInfo>();
BOOL ObjectManager::flag_isObjectInfoInit = FALSE;

int ObjectInfo::count_texture()
{
	return texture.name.size();
}
int ObjectInfo::count_texture_size()
{
	return texture.size.size();
}



BOOL RollingBall::ObjectManager::init(HWND hwnd)
{
	if (isInitObjectInfo()) return TRUE;
	if (init_object_info(hwnd)) {
		flag_isObjectInfoInit = TRUE;
		return TRUE;
	}
	else return FALSE;
}
BOOL RollingBall::ObjectManager::init_object_info(HWND hwnd)
{
	LPCTSTR filename = _T("..\\res\\bmp\\object_info.txt");
	FileManager file;

	if (!file.open(filename, _T("r")))
	{
		TCHAR errmsg[256];
		_stprintf_s(errmsg, 256, _T("%s 파일을 열 수 없습니다."), filename);
		MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
		return FALSE;
	}

	//파일을 한 줄씩 읽어 object에 저장한다.
	TCHAR line[128];
	BOOL isObjectReading = FALSE;
	int objidx = -1;
	while (file.readLine(line, sizeof(line)))
	{
		//공백 라인이거나 주석일 경우 패스
		if (_tcslen(line) == 0) continue;
		if (line[0] == _T('#')) continue;

		//오브젝트 정보를 읽는 중이 아닐 때...
		if (!isObjectReading) {

			//오브젝트 개수를 설정함
			if (_tcscmp(line, _T("object_count=")) == 0) {
				file.readLine(line, sizeof(line));
				object_info.resize(_ttoi(line));
			}

			//오브젝트 정보를 읽기 시작함
			if (_tcscmp(line, _T("<object>")) == 0) {
				objidx++;
				isObjectReading = TRUE;
				continue;
			}
		}

		//오브젝트 정보를 읽는 중일 때..
		if (isObjectReading)
		{
			//오브젝트의 이름을 저장
			if (_tcscmp(line, _T("name=")) == 0) {
				file.readLine(line, sizeof(line));
				object_info[objidx].name = line;
				continue;
			}

			//오브젝트 텍스쳐 개수 설정
			if (_tcscmp(line, _T("texture_count=")) == 0) {
				file.readLine(line, sizeof(line));
				object_info[objidx].texture.name.resize(_ttoi(line));
				continue;
			}

			//오브젝트 텍스쳐 이름 저장
			if (_tcscmp(line, _T("texture_name=")) == 0) {
				for (int i = 0; i < object_info[objidx].count_texture(); i++)
				{
					file.readLine(line, sizeof(line));
					object_info[objidx].texture.name[i] = line;
				}
				continue;
			}

			//오브젝트 텍스쳐 크기 값 개수 설정
			if (_tcscmp(line, _T("texture_size_count=")) == 0) {
				file.readLine(line, sizeof(line));
				object_info[objidx].texture.size.resize(_ttoi(line));
				continue;
			}

			//오브젝트 텍스쳐 크기 값 저장
			if (_tcscmp(line, _T("texture_size_value=")) == 0) {
				for (int i = 0; i < object_info[objidx].count_texture_size(); i++) {
					file.readLine(line, sizeof(line));
					object_info[objidx].texture.size[i] = _ttoi(line);
				}
				continue;
			}

			//오브젝트의 마스크 여부 저장
			if (_tcscmp(line, _T("has_mask=")) == 0) {
				file.readLine(line, sizeof(line));
				if (_tcscmp(line, _T("TRUE")) == 0)
					object_info[objidx].has_mask = TRUE;
				if (_tcscmp(line, _T("FALSE")) == 0)
					object_info[objidx].has_mask = FALSE;
				continue;
			}
		}

		//오브젝트 정보를 모두 읽었음
		if (_tcscmp(line, _T("</object>")) == 0) {
			isObjectReading = FALSE;
			continue;
		}

		//파일이 종료됨
		if (_tcscmp(line, _T("</object_info.txt>")) == 0)
			break;
	}

	//파일을 닫는다.
	file.close();
	return TRUE;
}
BOOL RollingBall::ObjectManager::isInitObjectInfo()
{
	return flag_isObjectInfoInit;
}
