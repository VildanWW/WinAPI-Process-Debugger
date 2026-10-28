#pragma once
#include <Windows.h>

namespace ProcessDebugger {
	namespace Core {
		class ProcessHandler {
		private:
			HANDLE hProcess;
			DWORD procId;
			HWND hwnd;

			uintptr_t FindModuleAddress(DWORD procId, const wchar_t* modName);

			bool AttachToProcess();
		public:
			ProcessHandler();

			~ProcessHandler();

			bool Initializiting();
			void Shutdown();

			HANDLE GetHandle() const { return hProcess; }
			DWORD GetPid() const { return procId; }
			HWND GetHwnd() const { return hwnd; }
		};
	}
}