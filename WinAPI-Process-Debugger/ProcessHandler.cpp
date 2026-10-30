#include <Windows.h>
#include <TlHelp32.h>

#include "ProcessHandler.h"
#include "Settings.h"
#include "Offsets.h"

namespace ProcessDebugger {
	namespace Core {

		ProcessHandler::ProcessHandler() : hProcess(nullptr), procId(0), hwnd(nullptr) {}

		ProcessHandler::~ProcessHandler() { Shutdown(); }

		bool ProcessHandler::AttachToProcess() {
			HWND hwndProc = FindWindowA(NULL, Settings::nameProcess);

			if (!hwndProc) return false;

			DWORD procIdGame;
			GetWindowThreadProcessId(hwndProc, &procIdGame);
			
			HANDLE hProcGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procIdGame);
			
			if (!hProcGame || hProcGame == INVALID_HANDLE_VALUE) return false;

			hProcess = hProcGame;
			procId = procIdGame;
			hwnd = hwndProc;

			return true;
		}

		bool ProcessHandler::Initializiting() {

			if (!AttachToProcess()) return false;

			Offsets::clientBase = FindModuleAddress(procId, Settings::nameClient);
			Offsets::hwBase = FindModuleAddress(procId, Settings::nameHw);

			if (!Offsets::clientBase || !Offsets::hwBase) {
				Shutdown();
				return false;
			}

			return true;
		}

		uintptr_t ProcessHandler::FindModuleAddress(DWORD procId, const wchar_t* moduleName) {
			uintptr_t moduleBaseAddr = 0;
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

			if (hSnap == INVALID_HANDLE_VALUE) {
				return 0;
			}

			MODULEENTRY32W moduleEntry{};

			moduleEntry.dwSize = sizeof(moduleEntry);

			if (Module32FirstW(hSnap, &moduleEntry)) {
				do {
					if (!_wcsicmp(moduleEntry.szModule, moduleName)) {
						moduleBaseAddr = (uintptr_t)moduleEntry.modBaseAddr;
						break;
					}
				} while (Module32NextW(hSnap, &moduleEntry));
			}

			CloseHandle(hSnap);
			return moduleBaseAddr;
		}
	
		void ProcessHandler::Shutdown() {
			if (hProcess && hProcess != INVALID_HANDLE_VALUE) {
				CloseHandle(hProcess);
			}

			hProcess = nullptr;
			procId = 0;
			hwnd = nullptr;
		}
	}
}
