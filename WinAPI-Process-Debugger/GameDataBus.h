#pragma once
#include "GameStructures.h"
#include <mutex>

namespace ProcessDebugger {
	namespace Core {
		class GameDataBus {
		private:
			Data::GameShapshot bufferA;
			Data::GameShapshot bufferB;

			Data::GameShapshot* writeBuffer = &bufferA;
			Data::GameShapshot* readBuffer = &bufferB;

			std::mutex swapMutex;
			std::atomic<bool> hasNewData = false;

			std::atomic<int> lockedTargetIndex = -1;
		public:
			void PublishSnapshot(const Data::GameShapshot& snapshot);

			bool FetchLatestSnapshot(Data::GameShapshot& outSnapshot);

			int GetLockedTargetIndex() const;
			void SetLockedTargetIndex(int index);
		};
	}
}


