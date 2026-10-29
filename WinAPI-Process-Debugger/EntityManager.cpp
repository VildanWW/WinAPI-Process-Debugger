#include <Windows.h>
#include <thread>

#include "EntityManager.h"
#include "GameDataBus.h"
#include "ProcessHandler.h"
#include "Offsets.h"
#include "Settings.h"

namespace ProcessDebugger {
	namespace Core {
		bool EntityManager::IsPlayerActive(int indexPlayer) {
			uintptr_t addr = Offsets::hwBase + Offsets::positionPlayers + (indexPlayer * 0x280) + 0xA0;
			unsigned char currentByte = 0;

			if (ReadProcessMemory(handler->GetHandle(), reinterpret_cast<LPCVOID>(addr), &currentByte, 1, nullptr)) {
				if (currentByte != lastPulse[indexPlayer]) {
					lastPulse[indexPlayer] = currentByte; 
					return true; 
				}
			}
			return false;
		}

		EntityManager::EntityManager(ProcessHandler* processHandler) : handler(processHandler), lastPulse{ 0 } {}

		void EntityManager::MemoryLoop(GameDataBus& dataBus) {
			while (Settings::running.load()) {
				int currentLockedTarget = dataBus.GetLockedTargetIndex();

				Data::GameShapshot snapshot = GameSnapshotUpdate(currentLockedTarget);

				dataBus.PublishSnapshot(snapshot);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		Data::GameShapshot EntityManager::GameSnapshotUpdate(int lockedTargetIdx) {
			Data::GameShapshot snapshot;

			snapshot.lockedTargetIndex = lockedTargetIdx;

			HANDLE hProcess = handler->GetHandle();

			uintptr_t myPosAddr = Offsets::hwBase + Offsets::positionPlayers - 0x1E0 + 0x70;
			ReadProcessMemory(hProcess, (LPCVOID)myPosAddr, &snapshot.myPosition, sizeof(snapshot.myPosition), nullptr);
			ReadProcessMemory(hProcess, (LPCVOID)(Offsets::hwBase + Offsets::viewMatrix), &snapshot.viewMatrix, sizeof(snapshot.viewMatrix), nullptr);

			int myState = 0;
			ReadProcessMemory(hProcess, (LPCVOID)(Offsets::hwBase + Offsets::myState), &myState, sizeof(myState), nullptr);
		
			int myIndex = myState - 1;
			int myTeam = 0;

			ReadProcessMemory(hProcess, (LPCVOID)(Offsets::clientBase + Offsets::teamStatePlayers + (myIndex * 0xC0)), &myTeam, sizeof(myTeam), nullptr);

			for (int i = 0; i < Data::countPlayers; i++) {
				if (i == myIndex) continue;
				if (!IsPlayerActive(i)) continue;

				Data::PlayerData p;
				int enemyTeam = 0;

				ReadProcessMemory(hProcess, (LPCVOID)(Offsets::hwBase + Offsets::statePlayers + (i * 0x28)), &p.state, sizeof(p.state), nullptr);
				ReadProcessMemory(hProcess, (LPCVOID)(Offsets::hwBase + Offsets::positionPlayers + (i * 0x280) + 0xA8), &p.pos, sizeof(p.pos), nullptr);
				ReadProcessMemory(hProcess, (LPCVOID)(Offsets::clientBase + Offsets::teamStatePlayers + (i * 0xC0)), &enemyTeam, sizeof(char), nullptr);
			
				if (p.state > 0 && p.pos.x != 0.0f && enemyTeam != myTeam) {

					uintptr_t yawAddr = (i == 0) ? (Offsets::hwBase + Offsets::positionPlayers - 0x1CC + 0x4) : (Offsets::hwBase + Offsets::positionPlayers + (i * 0x280) + 0xA4);
					ReadProcessMemory(hProcess, (LPCVOID)(yawAddr), &p.yaw, sizeof(p.yaw), nullptr);

					p.index = i;

					snapshot.players.push_back(p);
				}
			}
			return snapshot;
		}
	}
}