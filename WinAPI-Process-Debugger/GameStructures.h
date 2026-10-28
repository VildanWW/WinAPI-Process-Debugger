#pragma once
#include <vector>

#include "ProjectMath.h"

namespace ProcessDebugger {
	namespace Data {
		struct PlayerData {
			ProjectMath::Vector3 pos;
			int state;
			int team;
			int index;
			int yaw;
		};

		struct GameShapshot {
			ProjectMath::ViewMatrix viewMatrix;
			ProjectMath::Vector3 myPosition;
			std::vector<PlayerData> players;
			int lockedTargetIndex = -1;
		};

		inline constexpr size_t countPlayers = 32; // basic count people = 32
	}
}