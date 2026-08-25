#pragma once
#include "IBaseFeature.h"
#include "Offsets.h"
#include "Memory.h"
namespace ProcessDebugger {
	namespace Features {
        class InputAutomation : public IBaseFeature {
        private:
            DWORD jump;
            DWORD air;
        public:
            InputAutomation();

            void UpDate() override;
            void MenuRender() override;

            ~InputAutomation() override = default;
        };
	}
}