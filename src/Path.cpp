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

bool Lightup::Path::CombinePath(const std::wstring &a, const std::wstring &b, std::wstring &result)
{
	// PathCombine() 函数无法处理长度超过 MAX_PATH 个字符的路径
	if (a.length() > (MAX_PATH - 1) || b.length() > (MAX_PATH - 1)) return false;

	wchar_t buff[MAX_PATH];
	ZeroMemory(buff, MAX_PATH);

	if (PathCombineW(buff, a.c_str(), b.c_str()) == NULL)
	{
		return false;
	}

	result = buff;
	return true;
}

bool Lightup::Path::CanonicalizePath(const std::wstring &path, std::wstring &result)
{
	// PathCanonicalize() 函数无法处理长度超过 MAX_PATH 个字符的路径
	if (path.length() > (MAX_PATH - 1)) return false;

	wchar_t buff[MAX_PATH];
	ZeroMemory(buff, MAX_PATH);

	if (PathCanonicalizeW(buff, path.c_str()) == NULL)
	{
		return false;
	}

	result = buff;
	return true;
}

// max 只包括字符串本身的长度，不包括结尾 NULL
bool Lightup::Path::CompactPath(const std::wstring &path, int max, std::wstring &result)
{
	if (max < 0 || max > (MAX_PATH-1)) return false;

	wchar_t buff[MAX_PATH];
	ZeroMemory(buff, MAX_PATH);

	if (PathCompactPathEx(buff, path.c_str(), max+1, 0) == FALSE)
	{
		return false;
	}

	result = buff;
	return true;
}
