#pragma once
#include "IBaseFeature.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

namespace ProcessDebugger {
	namespace Features {
		class EntityVisualizer : public IBaseFeature {
		private:
			RECT gRect;
			HWND ovl;

			IDXGISwapChain* swapChain = nullptr;
			ID3D11Device* d3dDevice = nullptr;
			ID3D11DeviceContext* d3dDeviceContext = nullptr;
			ID3D11RenderTargetView* mainRenderTargetView = nullptr;

			int sW;
			int sH;
		public:
			EntityVisualizer();

			void Initialization();

			void UpDate() override;
			void MenuRender() override;

			~EntityVisualizer();
		};
	}
}