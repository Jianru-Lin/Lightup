#pragma once

#include <string>

class Process {
public:
	static bool TerminateByName(const std::wstring &name);
	static bool Run(const std::wstring &fileName);
};