#pragma once
#include <vector>
#include <memory>

#include "IBaseFeature.h"
#include "ProcessHandler.h"

namespace ProcessDebugger {
	namespace Features {
		class FeatureManager {
		private:
			std::vector<std::unique_ptr<IBaseFeature>> features;

			Core::ProcessHandler* processHandler;
		public:
			FeatureManager(Core::ProcessHandler* processHandler);
			void UpdateLoop();
		};
	}
}

