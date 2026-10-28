#pragma once
#include "IBaseFeature.h"
#include "Offsets.h"

namespace ProcessDebugger {
	namespace Features {
        class InputAutomation : public IBaseFeature {
        private:
            uintptr_t jump;
            uintptr_t air;
        public:
            InputAutomation();

            void UpDate() override;
            void MenuRender() override;
        };
	}
}