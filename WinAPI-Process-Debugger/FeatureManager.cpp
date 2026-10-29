#include <memory>
#include <thread>

#include "FeatureManager.h"
#include "InputAutomation.h"
#include "ProcessHandler.h"
#include "Settings.h"

namespace ProcessDebugger {
	namespace Features {
		FeatureManager::FeatureManager(Core::ProcessHandler* processHandler) : processHandler(processHandler) {
			features.push_back(std::make_unique<InputAutomation>());

			for (int i = 0; i < features.size(); i++) {
				features[i]->SetProcessHandler(processHandler);
			}
		}

		void FeatureManager::UpdateLoop() {
            while (Settings::running.load()) {
				{
					std::unique_lock<std::mutex> lock(mtxCycleRun);
					cvCycleRun.wait(lock, [this]() {
						return !features.empty() || !Settings::running;
					});
				}

				if (!Settings::running.load()) break;

                for (const auto& feature : features) {
                    if (feature->GetEnabled()) {
                        feature->UpDate();
                    }
                }
            }
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		FeatureManager::~FeatureManager() {};
	}
}
