#include <dwmapi.h>

#include "EntityVisualizer.h"
#include "ProcessHandler.h"
#include "GameStructures.h"
#include "GameDataBus.h"
#include "ProjectMath.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#pragma comment(lib, "dwmapi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK OverlayWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

namespace ProcessDebugger {
	namespace Features {
        EntityVisualizer::EntityVisualizer() : IBaseFeature(true, true, FeatureID::Visualizer) {}

        void EntityVisualizer::Initialization() {
            HWND gameHwnd = handler->GetHwnd();

            GetWindowRect(gameHwnd, &gRect);

            sW = gRect.right - gRect.left;
            sH = gRect.bottom - gRect.top;

            WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_CLASSDC, OverlayWndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "OverlayClass", NULL };
            RegisterClassExA(&wc);

            ovl = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, "OverlayClass", "Overlay", WS_POPUP, gRect.left, gRect.top, sW, sH, NULL, NULL, wc.hInstance, NULL);

            SetLayeredWindowAttributes(ovl, RGB(0, 0, 0), 255, LWA_COLORKEY);

            MARGINS margins = { -1, -1, -1, -1 };
            DwmExtendFrameIntoClientArea(ovl, &margins);

            ShowWindow(ovl, SW_SHOW);

            DXGI_SWAP_CHAIN_DESC sd = {};
            sd.BufferCount = 1;
            sd.BufferDesc.Width = sW;
            sd.BufferDesc.Height = sH;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = ovl;
            sd.SampleDesc.Count = 1;
            sd.Windowed = TRUE;
            sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

            D3D_FEATURE_LEVEL featureLevel;
            D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &swapChain, &d3dDevice, &featureLevel, &d3dDeviceContext);

            ID3D11Texture2D* pBackBuffer = nullptr;
            swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            ImGui_ImplWin32_Init(ovl);
            ImGui_ImplDX11_Init(d3dDevice, d3dDeviceContext);
            ImGui::StyleColorsDark();
        }

        void EntityVisualizer::UpDate() {
            MSG msg;
            while (PeekMessageA(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
                if (msg.message == WM_QUIT) return;
            }
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            d3dDeviceContext->ClearRenderTargetView(mainRenderTargetView, clearColor);
            d3dDeviceContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

            ImDrawList* drawList = ImGui::GetBackgroundDrawList();

            Data::GameShapshot gameSnapshot;

            gameDataBus->FetchLatestSnapshot(gameSnapshot);

            for (const auto& p : gameSnapshot.players) {
                POINT sc; float d;
                if (ProjectMath::WorldToScreen(p.pos, gameSnapshot.viewMatrix, sc, sW, sH, d)) {
                    int w = (int)(15000 / d);
                    int h = (int)(30000 / d);

                    drawList->AddRect(
                        ImVec2(sc.x - w / 2.0f, sc.y - h),
                        ImVec2(sc.x + w / 2.0f, sc.y),
                        IM_COL32(255, 0, 0, 255),
                        0.0f, 0, 1.5f
                    );
                }
            }

            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            swapChain->Present(1, 0);
        }

        void EntityVisualizer::MenuRender() {}

        EntityVisualizer::~EntityVisualizer() {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();

            if (mainRenderTargetView) mainRenderTargetView->Release();
            if (swapChain) swapChain->Release();
            if (d3dDeviceContext) d3dDeviceContext->Release();
            if (d3dDevice) d3dDevice->Release();

            DestroyWindow(ovl);
        }
    }
}

