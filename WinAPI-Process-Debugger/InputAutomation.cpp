#include "InputAutomation.h"
#include "Offsets.h"
#include "ProcessHandler.h"
#include <Windows.h>
#include "Settings.h"

namespace ProcessDebugger {
	namespace Features {
		InputAutomation::InputAutomation() : IBaseFeature(true, true), jump(0), air(0) {}

		void InputAutomation::UpDate() {
			if (GetAsyncKeyState(Settings::bhop)) {
				int airState;
				if (jump == 0 || air == 0) {
					jump = Offsets::clientBase + Offsets::responseJump;
					air = Offsets::clientBase + Offsets::flagJump;
				}
				ReadProcessMemory(handler->GetHandle(), (LPCVOID)air, &airState, sizeof(airState), NULL);

				if (airState == 0) {
					const int valueFive = 5;
					const int valueFour = 4;

					WriteProcessMemory(handler->GetHandle(), (LPVOID)jump, &valueFive, sizeof(valueFive), NULL);

					Sleep(5);

					WriteProcessMemory(handler->GetHandle(), (LPVOID)jump, &valueFour, sizeof(valueFive), NULL);
				}
			}
		}

		void InputAutomation::MenuRender() {}
	}
}


