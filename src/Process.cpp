#include "Process.h"
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

	// ע�⣺
	// 1�����������Ľ��̿��ܻ��ж��
	// 2����ֹ�κ�һ������ʱ�����ܻ��������ʹ������Ȼ�������ֹ��һ������

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
