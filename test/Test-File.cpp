#include <iostream>
#include "File.h"

using namespace std;
using namespace Lightup;

bool test()
{
	wstring content;
	bool success = File::ReadUtf8File(L"h:\\test.txt", content);
	return success;
}

int main() 
{
	bool s = File::MoveFile(L"h:\\test2.txt", L"g:\\test.txt");
	cout << s << endl;
	return 0;
}