#include "Memory.h"
#include <TlHelp32.h>
#include <iostream>

bool ProcessDebugger::Core::AttachToProcess() {
	HWND hwnd = FindWindowA(NULL, "Counter-Strike");

	if (!hwnd) {
		std::cout << "Game not found!" << std::endl;
		return false;
	}

	DWORD procId;
	GetWindowThreadProcessId(hwnd, &procId);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	Core::procId = procId;
	Core::hwnd = hwnd;

	return true;
}

uintptr_t ProcessDebugger::Core::FindModuleAddress(DWORD procId, const wchar_t* moduleName) {
	uintptr_t moduleBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32W moduleEntry;

		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32FirstW(hSnap, &moduleEntry)) {
			do {
				if (!_wcsicmp(moduleEntry.szModule, moduleName)) {
					moduleBaseAddr = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32NextW(hSnap, &moduleEntry));
		}
	}

	CloseHandle(hSnap);
	return moduleBaseAddr;
}
