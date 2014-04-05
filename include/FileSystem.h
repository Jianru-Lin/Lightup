#pragma once

#include <string>

class FileSystem {
public:
	static bool ReadUtf8File(const std::wstring &filename, std::wstring &content);
	static bool ExpandPath(const std::wstring &path, std::wstring &result);
};