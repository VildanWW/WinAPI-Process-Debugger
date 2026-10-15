#include <Windows.h>
#include <vector>
#include <thread>
#include <memory>
#include "Memory.h"
#include "InputAutomation.h"
#include "EntityVisualizer.h"
#include "VectorController.h"
#include "IBaseFeature.h"
#include "Menu.h"
#include "StabilizationSystem.h"
#include "Settings.h"
#include <typeinfo>

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

namespace EF = ProcessDebugger::Features;
namespace EC = ProcessDebugger::Core;

int main() {
	if (!EC::AttachToProcess()) return 0;

	std::vector<std::thread> threads;
	EC::clientBase = EC::FindModuleAddress(EC::procId, L"client.dll");
	EC::hwBase = EC::FindModuleAddress(EC::procId, L"hw.dll");

	feature.push_back(std::make_unique<EF::InputAutomation>());
	feature.push_back(std::make_unique<EF::EntityVisualizer>());
	feature.push_back(std::make_unique<EF::VectorController>());
	feature.push_back(std::make_unique<EF::StabilizationSystem>());

	for (auto& f : feature) {
		if (f->getBackGround()) {
			auto rawPtr = f.get();
			threads.push_back(std::thread([rawPtr]() {
				while (!Settings::bShouldQuit) {
					if (rawPtr->getEnabled()) {
						rawPtr->UpDate();
					}
					if (typeid(*rawPtr) != typeid(EF::InputAutomation)) {
						Sleep(1);
					}else std::this_thread::yield();
				}
			}));
		}
	}

	threads.push_back(std::thread(Menu::MenuThread));

	while (!Settings::bShouldQuit) {
		ProcessDebugger::Update();
		for (auto& f : feature) {
			if (!f->getBackGround() && f->getEnabled()) {
				f->UpDate();
			}
		}
		Sleep(1);
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}

	return 0;
}