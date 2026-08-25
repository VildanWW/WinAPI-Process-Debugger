#pragma once
#include "IBaseFeature.h"
#include "Offsets.h"
#include "Memory.h"
#include <thread>

namespace ProcessDebugger {
	namespace Features {
		class StabilizationSystem : public IBaseFeature {
		private:
			float prevPunchZ = 0.0f;
			float recoilFactor = 1.2f;
		public:
			StabilizationSystem();

			void UpDate() override;
			void MenuRender() override;

			~StabilizationSystem() override = default;
		};
	}
}