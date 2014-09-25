#include "Process.h"

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

using namespace std;

bool process_name_is(const PROCESSENTRY32 &process_entry, const wstring &name) {
	// wcout << process_entry.szExeFile << endl;
	bool match = _wcsicmp(process_entry.szExeFile, name.c_str()) == 0;
	return match;
}

bool terminate_process(const PROCESSENTRY32 &process_entry) {
	HANDLE h_process = OpenProcess(PROCESS_TERMINATE, FALSE, process_entry.th32ProcessID); // MARK 1
	if (h_process == NULL) return false;

	if (!TerminateProcess(h_process, 0)) {
		goto FREE_1;
	}
	else {
		// wcout << L"Terminate ok: " << process_entry.szExeFile << endl;
		CloseHandle(h_process);
		return true;
	}

FREE_1:
	CloseHandle(h_process);

	wcout << L"Terminate failed: " << process_entry.szExeFile << endl;

	return false;
}

bool Process::TerminateByName(const wstring &name)
{
	HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	// MARK 1
	if (h_snapshot == INVALID_HANDLE_VALUE) return false;

	PROCESSENTRY32 process_entry;
	ZeroMemory(&process_entry, sizeof(process_entry));
	process_entry.dwSize = sizeof(process_entry);

	if (!Process32First(h_snapshot, &process_entry)) goto FREE_1;

	// 注意：
	// 1、符合条件的进程可能会有多个
	// 2、终止任何一个进程时都可能会出错，但即使出错，依然会继续终止下一个进程

	bool terminate_all_success = false;

	if (process_name_is(process_entry, name)) {
		terminate_all_success = terminate_process(process_entry);
	}

	while (Process32Next(h_snapshot, &process_entry)) {
		if (process_name_is(process_entry, name)) {
			terminate_all_success = terminate_process(process_entry);
		}
	}

	return terminate_all_success;

FREE_1:
	CloseHandle(h_snapshot);

	return false;
}

bool Process::Run(const std::wstring &fileName)
{
	DWORD error = 0;

	wchar_t* fileNameCopy = (wchar_t*)malloc((fileName.length() + 1) * sizeof(wchar_t)); // MARK 1
	if (fileNameCopy == NULL) return false;

	fileName.copy(fileNameCopy, fileName.length());
	fileNameCopy[fileName.length()] = 0;

	STARTUPINFO startup = { 0 };
	startup.cb = sizeof(startup);
	PROCESS_INFORMATION info = { 0 };
	BOOL ret = CreateProcess(
		NULL,
		fileNameCopy,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&startup,
		&info);

	if (ret != 0)
	{
		CloseHandle(info.hProcess);
		CloseHandle(info.hThread);
	}
	else
	{
		error = GetLastError();
	}

	free(fileNameCopy);
	return ret != 0 ? true : false;
}