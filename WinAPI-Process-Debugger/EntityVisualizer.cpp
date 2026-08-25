#include "EntityVisualizer.h"
#include "IBaseFeature.h"
#include "Memory.h"
#include "Offsets.h"
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

ProcessDebugger::Features::EntityVisualizer::EntityVisualizer() : IBaseFeature(false, false) {
	GetWindowRect(Core::hwnd, &gRect);

	sW = gRect.right - gRect.left;
	sH = gRect.bottom - gRect.top;

	ovl = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, "Static", "Overlay", WS_POPUP, gRect.left, gRect.top, sW, sH, NULL, NULL, NULL, NULL);
	SetLayeredWindowAttributes(ovl, RGB(0, 0, 0), 255, LWA_COLORKEY);
	ShowWindow(ovl, SW_SHOW);

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	d3dpp = { 0 };
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = ovl;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, ovl, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);
}

void ProcessDebugger::Features::EntityVisualizer::UpDate() {

	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);
	d3dDevice->BeginScene();

	if (this->getEnabled()) {
		for (auto& p : players) {
			POINT sc; float d;
			if (WorldToScreen(p, sc, sW, sH, d)) {
				DrawEspBox(sc.x, sc.y, (int)(15000 / d), (int)(30000 / d), 0xFFFF0000);
			}
		}
	}

	d3dDevice->EndScene();
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void ProcessDebugger::Features::EntityVisualizer::DrawLine(float x1, float y1, float x2, float y2, DWORD color) {
	D3DVertex v[] = { {x1, y1, 0, 1, color}, {x2, y2, 0, 1, color} };
	d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	d3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(D3DVertex));
}

void ProcessDebugger::Features::EntityVisualizer::DrawEspBox(int x, int y, int w, int h, DWORD color) {
	DrawLine(x - w / 2, y - h, x + w / 2, y - h, color); 
	DrawLine(x - w / 2, y, x + w / 2, y, color);         
	DrawLine(x - w / 2, y - h, x - w / 2, y, color);    
	DrawLine(x + w / 2, y - h, x + w / 2, y, color);
}

void ProcessDebugger::Features::EntityVisualizer::MenuRender() {}

ProcessDebugger::Features::EntityVisualizer::~EntityVisualizer() {
	d3dDevice->Release();
	d3d->Release();
	DestroyWindow(ovl);
}
