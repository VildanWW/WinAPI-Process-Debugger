#include <Windows.h>

#include "VectorController.h"
#include "Settings.h"
#include "Offsets.h"
#include "ProcessHandler.h"

//namespace ProcessDebugger {
//	namespace Features {
//		VectorController::VectorController() : IBaseFeature(true, false) {}
//
//		void VectorController::UpDate() {
//			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
//				bool found = false;
//				float targetAng[2] = { 0,0 };
//
//				float currentViewAngles[2];
//				ReadProcessMemory(handler->GetHandle(), (LPCVOID)(Offsets::hwBase + Offsets::viewAngles), &currentViewAngles, sizeof(targetAng), NULL);
//
//				if()
//			}
//		}
//	}
//}


//ProcessDebugger::Features::VectorController::VectorController() : IBaseFeature(false, false) {}
//
//void ProcessDebugger::Features::VectorController::UpDate() {
//	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
//		bool found = false;
//		float targetAng[2] = { 0, 0 };
//
//		float currentViewAngles[2];
//		ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::hwBase + Offsets::viewAngles), &currentViewAngles, 8, NULL);
//
//		if (lockedTargetIndex == -1 && !players.empty()) {
//			float bestFov = 10.0f;
//
//			for (const auto& p : players) {
//				float rad = (p.yaw + 1.0f) * (M_PI / 180.0f);
//				float dX = (p.pos.x + cosf(rad)) - myPosition.x;
//				float dY = (p.pos.y + sinf(rad)) - myPosition.y;
//
//				float dZ = (p.pos.z + 20.5f) - (myPosition.z + 18.0f);
//				float fDist2D = sqrtf(dX * dX + dY * dY);
//
//				float ang[2];
//				ang[0] = -atan2f(dZ, fDist2D) * (180.0f / M_PI);
//				ang[1] = atan2f(dY, dX) * (180.0f / M_PI);
//
//				float fovY = abs(ang[1] - currentViewAngles[1]);
//				if (fovY > 180) fovY = 360 - fovY;
//				float fov = sqrtf(powf(ang[0] - currentViewAngles[0], 2) + powf(fovY, 2));
//
//				if (fov < bestFov) {
//					bestFov = fov;
//					lockedTargetIndex = p.index;
//				}
//			}
//		}
//
//		if (lockedTargetIndex != -1) {
//			found = false;
//			for (const auto& p : players) {
//				if (p.index == lockedTargetIndex) {
//					float rad = (p.yaw + 1.0f) * (M_PI / 180.0f);
//					float dX = (p.pos.x + cosf(rad)) - myPosition.x;
//					float dY = (p.pos.y + sinf(rad)) - myPosition.y;
//
//					float dZ = (p.pos.z + 20.5f) - (myPosition.z + 18.0f);
//					float fDist2D = sqrtf(dX * dX + dY * dY);
//
//					targetAng[0] = -atan2f(dZ, fDist2D) * (180.0f / M_PI);
//					targetAng[1] = atan2f(dY, dX) * (180.0f / M_PI);
//
//					float diffY = abs(targetAng[1] - currentViewAngles[1]);
//					if (diffY > 180) diffY = 360 - diffY;
//					float currentFov = sqrtf(powf(targetAng[0] - currentViewAngles[0], 2) + powf(diffY, 2));
//
//					if (currentFov > 12.0f) {
//						lockedTargetIndex = -1;
//						break;
//					}
//
//					found = true;
//					break;
//				}
//			}
//
//			if (found) {
//				WriteProcessMemory(Core::hProcess, (LPVOID)(Core::hwBase + Offsets::viewAngles), &targetAng[0], 8, NULL);
//			}
//
//			else {
//				lockedTargetIndex = -1;
//			}
//		}
//	}
//
//	else {
//		lockedTargetIndex = -1;
//	}
//}
//
//void ProcessDebugger::Features::VectorController::MenuRender() {}
//
