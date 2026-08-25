#pragma once
#include "IBaseFeature.h"
#include "Memory.h"
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")
#define M_PI 3.14159265358979323846f

namespace ProcessDebugger {
	namespace Features {
		class EntityVisualizer : public IBaseFeature {
		private:
			RECT gRect;
			HWND ovl;
			D3DPRESENT_PARAMETERS d3dpp;
			IDirect3D9* d3d;
			IDirect3DDevice9* d3dDevice;
			struct D3DVertex { float x, y, z, rhw; DWORD color; };
			int sW;
			int sH;
		public:
			EntityVisualizer();

			void UpDate() override;
			void MenuRender() override;

			void DrawLine(float x1, float y1, float x2, float y2, DWORD color);
			void DrawEspBox(int x, int y, int w, int h, DWORD color);

			~EntityVisualizer();
		};
	}
}