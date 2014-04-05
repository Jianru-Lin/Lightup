#include "FileSystem.h"
#include <iostream>
#include <windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <climits>

#pragma comment(lib, "Shlwapi.lib")

using namespace std;

// ！注意目前没有妥善的处理 BOM 头问题，后续需要改进这一点
bool FileSystem::ReadUtf8File(const std::wstring &filename, std::wstring &content) {
	wstring filename_expanded;

	// 展开路径
	
	if (!FileSystem::ExpandPath(filename, filename_expanded)) {
		return false;
	}

	// 打开文件

	HANDLE h_file = CreateFile(
		filename_expanded.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);	// MARK 1

	if (h_file == INVALID_HANDLE_VALUE) goto RETURN_FALSE;

	// 获取文件尺寸

	LARGE_INTEGER li_size;
	ZeroMemory(&li_size, sizeof(li_size));

	if (!GetFileSizeEx(h_file, &li_size)) goto FREE_1;

	if (li_size.HighPart > 0) goto FREE_1; // 文件尺寸不能超过 4GB

	size_t buff_in_len = li_size.LowPart;

	if (buff_in_len == 0) { // 文件大小为 0，这种情况直接返回空字符串即可
		content = L"";
		CloseHandle(h_file);
		return true;
	}

	char *buff_in = (char*)malloc(buff_in_len);	// MARK 2
	if (buff_in == NULL) goto FREE_1;

	DWORD bytes_read = 0;

	if (!ReadFile(h_file, buff_in, buff_in_len, &bytes_read, NULL) || bytes_read != buff_in_len) goto FREE_2;

	if (bytes_read >= INT_MAX) goto FREE_2;	// 因为 MultiByteToWideChar 的限制，无法一次性转换尺寸超过 INT_MAX 的缓冲

	// 进行编码转换
	
	int buff_out_cch = MultiByteToWideChar(CP_UTF8, 0, buff_in, (int)bytes_read, NULL, 0);

	if (buff_out_cch == 0) goto FREE_2;

	wchar_t *buff_out = (wchar_t*)malloc((buff_out_cch+1)*sizeof(wchar_t)); // MARK 3
	if (buff_out == NULL) goto FREE_2;

	int result = MultiByteToWideChar(CP_UTF8, 0, buff_in, (int)bytes_read, buff_out, buff_out_cch);

	if (result == 0) goto FREE_3;

	// 将转码后的缓冲转为字符串

	buff_out[buff_out_cch] = 0; // 断尾

	content = buff_out;

	// 释放资源

	free(buff_out);			// FREE 1
	free(buff_in);			// FREE 2
	CloseHandle(h_file);	// FREE 3

	// 返回成功

	return true;

FREE_3:
	free(buff_out);

FREE_2:
	free(buff_in);

FREE_1:
	CloseHandle(h_file);

RETURN_FALSE:
	return false;
}

bool FileSystem::ExpandPath(const std::wstring &path, std::wstring &result) {
	const DWORD len = 1024;
	wchar_t *buff = (wchar_t*)malloc(len);
	if (ExpandEnvironmentStrings(path.c_str(), buff, len / 2)) {
		result = buff;
		free(buff);
		return true;
	} else {
		DWORD error = GetLastError();
		return false;
	}
}

//int main() {
//	//wstring content = L"";
//	//if (!FileSystem::ReadTempFile(L"~~login.info", content)) {
//	//	cout << "read failed" << endl;
//	//	return 0;
//	//}
//
//	//wcout << content << endl;
//
//	wstring v = L"?";
//	FileSystem::ReadUtf8File(L"H:\\哈哈.txt", v);
//	wcout << v;
//	wcout << endl;
//	return 0;
//}