#pragma once
#include <Windows.h>

namespace ProcessDebugger {
	namespace Core {
		inline HANDLE hProcess;
		inline DWORD procId;
		inline HWND hwnd;
		inline uintptr_t clientBase;
		inline uintptr_t hwBase;

		bool AttachToProcess();
		uintptr_t FindModuleAddress(DWORD procId, const wchar_t* modName);
	}
}