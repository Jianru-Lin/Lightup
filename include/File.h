#pragma once

#include <string>

namespace Lightup {
	namespace File {
		bool ReadUtf8File(const std::wstring &filename, std::wstring &content);
		bool ExpandPath(const std::wstring &path, std::wstring &result);
		bool DeleteFile(const std::wstring &filename);
		bool GetFileSize(const std::wstring &filename, long long &size);
		bool RenameFile(const std::wstring &filename, const std::wstring &new_name);
		bool MoveFile(const std::wstring &filename, const std::wstring &new_filename);
	}
}