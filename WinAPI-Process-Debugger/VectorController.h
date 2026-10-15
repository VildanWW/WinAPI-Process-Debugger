#pragma once
#include "IBaseFeature.h"

namespace ProcessDebugger {
	namespace Features {
		class VectorController : public IBaseFeature {
		private:
		public:
			VectorController();

			void UpDate() override;
			void MenuRender() override;
		};
	}
}