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
	}
}

namespace ProcessDebugger {
	namespace Features {
		class FeatureManager {
		private:
			std::vector<std::unique_ptr<IBaseFeature>> features;

			std::condition_variable cvCycleRun;
			std::mutex mtxCycleRun;

			Core::ProcessHandler* processHandler;
		public:
			FeatureManager(Core::ProcessHandler* processHandler);
			void UpdateLoop();
			~FeatureManager();
		};
	}
}

