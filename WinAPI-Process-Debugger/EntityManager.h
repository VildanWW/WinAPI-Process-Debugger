#pragma once

#include "GameStructures.h"

namespace ProcessDebugger {
	namespace Core {
		class GameDataBus;
		class ProcessHandler;
	}
}

namespace ProcessDebugger {
	namespace Core {
		class EntityManager {
		private:
			bool IsPlayerActive(int indexPlayer);

			ProcessHandler* handler;

			unsigned char lastPulse[Data::countPlayers];
		public:
			EntityManager(ProcessHandler* processHandler);

			void MemoryLoop(GameDataBus& dataBus);

			Data::GameShapshot GameSnapshotUpdate(int lockedTargetIdx);
		};
	}
}

