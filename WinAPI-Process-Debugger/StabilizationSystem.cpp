//#include "StabilizationSystem.h"
//
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
