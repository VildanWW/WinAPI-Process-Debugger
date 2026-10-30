#include <Windows.h>
#include "StabilizationSystem.h"
#include "Settings.h"
#include "ProcessHandler.h"

namespace ProcessDebugger {
	namespace Features {
		StabilizationSystem::StabilizationSystem() : IBaseFeature(true, true, FeatureID::NoRecoil) {}

		void StabilizationSystem::UpDate() {
			if (GetAsyncKeyState(Settings::clickAim) & 0x8000) {
				float currentPunch;
				ReadProcessMemory(handler->GetHandle(), (LPCVOID)(Offsets::hwBase + Offsets::punch), &currentPunch, sizeof(currentPunch), nullptr);
				float deltaZ = currentPunch - prevPunchZ;

				if (deltaZ < -0.01) {
					float currentPitch;
					ReadProcessMemory(handler->GetHandle(), (LPCVOID)(Offsets::hwBase + Offsets::viewAngles), &currentPitch, sizeof(currentPitch), nullptr);
		
					float newPitch = currentPitch - (deltaZ * recoilFactor);

					WriteProcessMemory(handler->GetHandle(), (LPVOID)(Offsets::hwBase + Offsets::viewAngles), &newPitch, sizeof(newPitch), nullptr);
				}

				prevPunchZ = currentPunch;
			} 

			else {
				prevPunchZ = 0.0f;
			}
		}
		void StabilizationSystem::MenuRender() {}
	}
}

//ProcessDebugger::Features::StabilizationSystem::StabilizationSystem() : IBaseFeature (false, true) {}
//
//void ProcessDebugger::Features::StabilizationSystem::UpDate() {
//	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
//		float currentPunch;
//		ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::hwBase + Offsets::punch), &currentPunch, 4, NULL);
//		float deltaZ = currentPunch - prevPunchZ;
//
//		if (deltaZ < -0.01) {
//			float currentPitch;
//			ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::hwBase + Offsets::viewAngles), &currentPitch, 4, NULL);
//		
//			float newPitch = currentPitch - (deltaZ * recoilFactor);
//
//			WriteProcessMemory(Core::hProcess, (LPVOID)(Core::hwBase + Offsets::viewAngles), &newPitch, 4, NULL);
//		}
//
//		prevPunchZ = currentPunch;
//	} 
//
//	else {
//		prevPunchZ = 0.0f;
//	}
//}
//
//void ProcessDebugger::Features::StabilizationSystem::MenuRender() {}
