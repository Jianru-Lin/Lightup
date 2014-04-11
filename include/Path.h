#pragma once

#include <string>

namespace Lightup {
	namespace Path {
		bool ExpandPath(const std::wstring &path, std::wstring &result);
		bool ParentPath(const std::wstring &path, std::wstring &result);
		bool CombinePath(const std::wstring &a, const std::wstring &b, std::wstring &result);
		bool CanonicalizePath(const std::wstring &path, std::wstring &result);
		bool CompactPath(const std::wstring &path, int max, std::wstring &result);
	}
}