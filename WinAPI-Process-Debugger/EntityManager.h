#pragma once

#include "GameStructures.h"
#include "ProcessHandler.h"
#include "GameDataBus.h"

namespace ProcessDebugger {
	namespace Core {
		class EntityManager {
		private:
			bool IsPlayerActive(int indexPlayer);

			ProcessHandler& handler;

			unsigned char lastPulse[Data::countPlayers];
		public:
			EntityManager(ProcessHandler& processHandler);

			void MemoryLoop(GameDataBus& dataBus);

			Data::GameShapshot GameSnapshotUpdate(int lockedTargetIdx);
		};
	}
}

