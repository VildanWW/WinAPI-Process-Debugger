#include <memory>
#include <thread>

#include "EntityManager.h"
#include "FeatureManager.h"
#include "ProcessHandler.h"
#include "GameDataBus.h"
#include "Settings.h"

using namespace ProcessDebugger::Features;
using namespace ProcessDebugger::Core;
using namespace ProcessDebugger::Settings;

int main() {
    ProcessHandler processHandler;
    if (!processHandler.Initializiting()) {
        return -1;
    }
    EntityManager entityManager(&processHandler);
    GameDataBus gameDataBus;
    
    FeatureManager featureManager(&processHandler, &gameDataBus);

    std::thread fd(&EntityManager::MemoryLoop, &entityManager, std::ref(gameDataBus));
    std::thread featuresThread(&FeatureManager::UpdateLoop, &featureManager);

    while (running.load()) {
        if (GetAsyncKeyState(VK_END)) {
            running.store(false);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (featuresThread.joinable()) {
        featuresThread.join();
    }
    if (fd.joinable()) {
        fd.join();
    }

    processHandler.Shutdown();
    return 0;
}