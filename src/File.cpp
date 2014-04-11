#include "File.h"
#include "Path.h"
#include <iostream>
#include <windows.h>
#include <climits>

// 因为 Windows.h 中定义了 DeleteFile 宏，但是我们又命名了同名的成员函数
// 为了避免冲突，这里必须取消其定义
// 其余类似
#undef DeleteFile
#undef GetFileSize
#undef MoveFile


using namespace std;
using namespace Lightup;

// ！注意目前没有妥善的处理 BOM 头问题，后续需要改进这一点
bool Lightup::File::ReadUtf8File(const std::wstring &filename, std::wstring &content) {
	wstring filename_expanded;

	// 展开路径
	
	if (!Path::ExpandPath(filename, filename_expanded)) {
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

	HANDLE h_heap = GetProcessHeap(); // 注意本句柄无需释放
	if (h_heap == NULL) goto FREE_1;

	char *buff_in = (char*)HeapAlloc(h_heap, HEAP_ZERO_MEMORY, buff_in_len);	// MARK 2
	if (buff_in == NULL) goto FREE_1;

	DWORD bytes_read = 0;

	if (!ReadFile(h_file, buff_in, buff_in_len, &bytes_read, NULL) || bytes_read != buff_in_len) goto FREE_2;

	if (bytes_read >= INT_MAX) goto FREE_2;	// 因为 MultiByteToWideChar 的限制，无法一次性转换尺寸超过 INT_MAX 的缓冲

	// 进行编码转换
	
	int buff_out_cch = MultiByteToWideChar(CP_UTF8, 0, buff_in, (int)bytes_read, NULL, 0);

	if (buff_out_cch == 0) goto FREE_2;

	wchar_t *buff_out = (wchar_t*)HeapAlloc(h_heap, HEAP_ZERO_MEMORY, (buff_out_cch+1)*sizeof(wchar_t)); // MARK 3
	if (buff_out == NULL) goto FREE_2;

	int result = MultiByteToWideChar(CP_UTF8, 0, buff_in, (int)bytes_read, buff_out, buff_out_cch);

	if (result == 0) goto FREE_3;

	// 将转码后的缓冲转为字符串

	buff_out[buff_out_cch] = 0; // 断尾

	content = buff_out;

	// 释放资源

	HeapFree(h_heap, 0, buff_out);			// FREE 1
	HeapFree(h_heap, 0, buff_in);			// FREE 2
	CloseHandle(h_file);	// FREE 3

	// 返回成功

	return true;

FREE_3:
	HeapFree(h_heap, 0, buff_out);

FREE_2:
	HeapFree(h_heap, 0, buff_in);

FREE_1:
	CloseHandle(h_file);

RETURN_FALSE:
	return false;
}

bool Lightup::File::DeleteFile(const std::wstring &filename) {

	// 展开路径
	
	wstring filename_expanded;

	if (!Path::ExpandPath(filename, filename_expanded)) return false;

	// 执行删除

	return ::DeleteFileW(filename_expanded.c_str()) == TRUE;
}

bool Lightup::File::GetFileSize(const std::wstring &filename, long long &size) {

	// 展开路径

	wstring filename_expanded;

	if (!Path::ExpandPath(filename, filename_expanded)) return false;

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

	size = li_size.QuadPart;

	CloseHandle(h_file);
	return true;

FREE_1:
	CloseHandle(h_file);

RETURN_FALSE:
	return false;
}

bool Lightup::File::RenameFile(const std::wstring &filename, const std::wstring &new_name) {

	// 展开路径

	wstring filename_expanded;

	if (!Path::ExpandPath(filename, filename_expanded)) return false;

	// 
	return MoveFileEx(filename.c_str(), new_name.c_str(), 0) == TRUE;
}

bool Lightup::File::MoveFile(const std::wstring &filename, const std::wstring &new_filename) {

	// 展开源路径

	wstring filename_expanded;

	if (!Path::ExpandPath(filename, filename_expanded)) return false;

	// 展开目标路径

	wstring new_filename_expanded;

	if (!Path::ExpandPath(new_filename, new_filename_expanded)) return false;

	// 移动

	return MoveFileEx(filename.c_str(), new_filename_expanded.c_str(), 0) == TRUE;
}