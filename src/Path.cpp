#include "Path.h"
#include <windows.h>
#include "Shlwapi.h"

#pragma comment(lib, "Shlwapi.lib")

using namespace std;

bool Lightup::Path::ExpandPath(const std::wstring &path, std::wstring &result)
{
	const DWORD len = 1024;
	wchar_t *buff = (wchar_t*)malloc(len);
	if (::ExpandEnvironmentStrings(path.c_str(), buff, len / 2)) 
	{
		result = buff;
		free(buff);
		return true;
	}
	else 
	{
		DWORD error = GetLastError();
		return false;
	}
}

bool Lightup::Path::ParentPath(const std::wstring &path, std::wstring &result)
{
	if (path.length() < 1) return false;

	wchar_t *tmp = _wcsdup(path.c_str());
	BOOL success = PathRemoveFileSpecW(tmp);
	if (success)
	{
		result = tmp;
	}

	return success == TRUE;
}
