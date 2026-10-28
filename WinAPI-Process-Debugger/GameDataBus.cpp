#include "GameDataBus.h"
#include "GameStructures.h"

#include <mutex>

namespace ProcessDebugger {
	namespace Core {
		void GameDataBus::PublishSnapshot(const Data::GameShapshot& snapshot) {
			*writeBuffer = snapshot;
			{
				std::lock_guard<std::mutex> lock(swapMutex);
				std::swap(writeBuffer, readBuffer);
				hasNewData = true;
			}
		}

		bool GameDataBus::FetchLatestSnapshot(Data::GameShapshot& outSnapshot) {
			if (!hasNewData) return false;
			{
				std::lock_guard<std::mutex> lock(swapMutex);
				outSnapshot = *readBuffer;
				hasNewData = false;
				return true;
			}
		}

		int GameDataBus::GetLockedTargetIndex() const { return lockedTargetIndex.load(); }
		void GameDataBus::SetLockedTargetIndex(int index) { lockedTargetIndex.store(index); }
	}
}