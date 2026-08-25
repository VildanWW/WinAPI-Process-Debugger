#pragma once
#include "Memory.h"
#include "Offsets.h"
#include <vector>
#define M_PI 3.14159265358979323846f

namespace ProcessDebugger {
	struct Vector3 { float x, y, z; };
	struct ViewMatrix { float m[16]; };
	struct PlayerData { Vector3 pos; int state; int team; int index; int yaw; };
	inline constexpr size_t countPlayers = 32;
	inline ViewMatrix vMatrix;
	inline Vector3 myPosition;
	inline int lockedTargetIndex = -1;
	inline std::vector<PlayerData> players;
	inline unsigned char lastPulse[countPlayers] = { 0 };

	inline bool isPlayerActive(int i) {
		uintptr_t addr = Core::hwBase + Offsets::positionPlayers + (i * 0x280) + 0xA0;
		unsigned char currentByte = 0;

		if (ReadProcessMemory(Core::hProcess, (LPCVOID)addr, &currentByte, 1, NULL)) {
			if (currentByte != lastPulse[i]) {
				lastPulse[i] = currentByte;
				return true;
			}
		}
		return false;
	}
	inline void Update() {
		players.clear();

		uintptr_t myPosAddr = Core::hwBase + Offsets::positionPlayers - 0x1E0 + 0x70;;
		ReadProcessMemory(Core::hProcess, (LPCVOID)myPosAddr, &myPosition, 12, NULL);
		ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::hwBase + Offsets::viewMatrix), &vMatrix, sizeof(vMatrix), NULL);
			
		int myState = 0;
		ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::hwBase + Offsets::myState), &myState, 4, NULL);
		int myIndex = myState - 1;
		int myTeam = 0;
		ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::clientBase + Offsets::teamStatePlayers + (myIndex * 0xC0)), &myTeam, 4, NULL);

		for (int i = 0; i < countPlayers; i++) {
			if (i == myIndex) continue;
			if (!isPlayerActive(i)) continue;

			PlayerData p;
			int enemyTeam = 0;

			ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::hwBase + Offsets::statePlayers + (i * 0x28)), &p.state, 4, NULL);
			ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::hwBase + Offsets::positionPlayers + (i * 0x280) + 0xA8), &p.pos, 12, NULL);
			ReadProcessMemory(Core::hProcess, (LPCVOID)(Core::clientBase + Offsets::teamStatePlayers + (i * 0xC0)), &enemyTeam, 1, NULL);

			if (p.state > 0 && p.pos.x != 0.0f && enemyTeam != myTeam) {
				uintptr_t yawAddr = (i == 0) ? (Core::hwBase + Offsets::positionPlayers - 0x1CC + 0x4) : (Core::hwBase + Offsets::positionPlayers + (i * 0x280) + 0xA4);
				ReadProcessMemory(Core::hProcess, (LPCVOID)yawAddr, &p.yaw, 4, NULL);
				p.index = i;
				players.push_back(p);
			}
		}
	}
	inline bool WorldToScreen(const PlayerData& player, POINT& out, int w, int h, float& dist) {
		float clipW = player.pos.x * vMatrix.m[3] + player.pos.y * vMatrix.m[7] + player.pos.z * vMatrix.m[11] + vMatrix.m[15];
		dist = clipW;
		if (clipW < 0.1f) return false;

		float x = (player.pos.x * vMatrix.m[0] + player.pos.y * vMatrix.m[4] + player.pos.z * vMatrix.m[8] + vMatrix.m[12]) / clipW;
		float y = (player.pos.x * vMatrix.m[1] + player.pos.y * vMatrix.m[5] + player.pos.z * vMatrix.m[9] + vMatrix.m[13]) / clipW;
	
		out.x = (int)((w / 2.0f) * (1 + x));
		out.y = (int)((h / 2.0f) * (1 - y));
		return true;
	}

	namespace Features {
		class IBaseFeature {
		protected: 			
			bool enabled;
			bool backGround;
		public:
			virtual void UpDate() = 0;
			virtual void MenuRender() = 0;

			bool getEnabled() {
				return enabled;
			}
			bool getBackGround() {
				return backGround;
			}
			bool* getEnabledPtr() {
				return &enabled;
			}
			
			void setEnabled(bool enabled) {
				this->enabled = enabled;
			}

			IBaseFeature(bool enabled = false, bool backGround = false) : enabled(enabled), backGround(backGround) {};

			virtual ~IBaseFeature() = default;
		};
	}
}