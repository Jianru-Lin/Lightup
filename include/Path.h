#pragma once

#include <string>

namespace Lightup {
	namespace Path {
		bool ExpandPath(const std::wstring &path, std::wstring &result);
		bool ParentPath(const std::wstring &path, std::wstring &result);
	}
}