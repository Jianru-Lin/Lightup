#include "Path.h"
#include <Shlwapi.h>
#include <iostream>

#pragma comment(lib, "Shlwapi.lib")

using namespace std;

int main() 
{
	wstring result;
	//Lightup::Path::ExpandPath(L"%temp%\\test.txt", result);
	//wcout << result << endl;

	bool ok = Lightup::Path::CompactPath(L"c:\\1234\\5678\\file.txt", 18, result);
	wcout << ok << endl;
	wcout << result << endl;

	return 0;
}