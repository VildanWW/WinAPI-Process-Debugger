#pragma once
#include <vector>
#include <memory>
#include <condition_variable>
#include <mutex>

namespace ProcessDebugger {
	namespace Features {
		class IBaseFeature;
	}
	namespace Core {
		class ProcessHandler;
		class GameDataBus;
	}
}

namespace ProcessDebugger {
	namespace Features {
		class FeatureManager {
		private:
			Core::ProcessHandler* processHandler = nullptr;
			Core::GameDataBus* gameDataBus = nullptr;

			std::vector<std::unique_ptr<IBaseFeature>> features;

			std::condition_variable cvCycleRun;
			std::mutex mtxCycleRun;
		public:
			FeatureManager(Core::ProcessHandler* processHandler, Core::GameDataBus* gameDataBus);
			void UpdateLoop();
			~FeatureManager();
		};
	}
}

