#include "InputAutomation.h"
#include "Memory.h"
#include "IBaseFeature.h"
#include <Windows.h>

ProcessDebugger::Features::InputAutomation::InputAutomation() : IBaseFeature(false, true), jump(Core::clientBase + Offsets::responseJump), air(Core::clientBase + Offsets::flagJump) {}

void ProcessDebugger::Features::InputAutomation::UpDate() {
	if (GetAsyncKeyState(VK_SPACE)) {
		int airState;

		ReadProcessMemory(Core::hProcess, (LPCVOID)air, &airState, 4, NULL);

		if (airState == 0) {
			const int valueFive = 5;
			const int valueFour = 4;

			WriteProcessMemory(Core::hProcess, (LPVOID)jump, &valueFive, 4, NULL);

			Sleep(5);

			WriteProcessMemory(Core::hProcess, (LPVOID)jump, &valueFour, 4, NULL);
		}
	}
}

void ProcessDebugger::Features::InputAutomation::MenuRender() {}


