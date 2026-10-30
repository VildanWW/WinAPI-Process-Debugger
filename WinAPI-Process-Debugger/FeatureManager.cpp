#include <memory>
#include <thread>

#include "InputAutomation.h"
#include "VectorController.h"
#include "StabilizationSystem.h"
#include "EntityVisualizer.h"

#include "FeatureManager.h"
#include "ProcessHandler.h"
#include "GameDataBus.h"
#include "Settings.h"
#include "IBaseFeature.h"

namespace ProcessDebugger {
	namespace Features {
		FeatureManager::FeatureManager(Core::ProcessHandler* processHandler, Core::GameDataBus* gameDataBus) : processHandler(processHandler), gameDataBus(gameDataBus) {
			features.push_back(std::make_unique<InputAutomation>());
			features.push_back(std::make_unique<VectorController>());
			features.push_back(std::make_unique<StabilizationSystem>());
			features.push_back(std::make_unique<EntityVisualizer>());

			for (int i = 0; i < features.size(); i++) {
				features[i]->SetProcessHandler(processHandler);
				features[i]->SetGameDataBus(gameDataBus);

				if (features[i]->GetFeatureId() == FeatureID::Visualizer) {
					auto featureWh = static_cast<EntityVisualizer*>(features[i].get());
					featureWh->Initialization();
				}
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
