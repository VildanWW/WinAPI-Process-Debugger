#include "Menu.h"
#include "Settings.h"

#pragma comment(lib, "dwmapi.lib")

// Mouse handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global variables
static bool s_bIsDragging = false;
static POINT s_DragStartMousePos = { 0, 0 };
static POINT s_DragStartWindowPos = { 0, 0 };
static bool s_bIsOverButton = false; // Check if hovering over button

LRESULT CALLBACK Menu::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Let ImGui handle messages first
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg) {
        case WM_LBUTTONDOWN: {
            // Get cursor position
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };

            // Check if we clicked the close button
            POINT screenPt = pt;
            ClientToScreen(hwnd, &screenPt);

            RECT windowRect;
            GetWindowRect(hwnd, &windowRect);

            // Close button area
            RECT closeButtonRect = {
                windowRect.right - 90,
                windowRect.top + 5,
                windowRect.right - 10,
                windowRect.top + 30
            };

            // If clicked close button, let ImGui handle it
            if (PtInRect(&closeButtonRect, screenPt)) {
                break;
            }

            // If clicked elsewhere, start dragging window
            s_bIsDragging = true;
            GetCursorPos(&s_DragStartMousePos);
            GetWindowRect(hwnd, reinterpret_cast<RECT*>(&s_DragStartWindowPos));

            SetCapture(hwnd);
            return 0;
        }

        case WM_MOUSEMOVE: {
            if (s_bIsDragging) {
                POINT pt;
                GetCursorPos(&pt);

                int newX = s_DragStartWindowPos.x + (pt.x - s_DragStartMousePos.x);
                int newY = s_DragStartWindowPos.y + (pt.y - s_DragStartMousePos.y);

                SetWindowPos(hwnd, NULL, newX, newY, 0, 0,
                           SWP_NOSIZE | SWP_NOZORDER);
                return 0;
            }
            break;
        }

        case WM_LBUTTONUP: {
            if (s_bIsDragging) {
                s_bIsDragging = false;
                ReleaseCapture();
                return 0;
            }
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Menu::Installation(HWND hwnd, IDirect3DDevice9* pDevice) {
    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(pDevice);

    // Style settings
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = false;

    // Customize style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowBorderSize = 2.0f;
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(10, 8);

    // Semi-transparent colors
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.95f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.95f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 0.95f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 0.95f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 0.95f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.95f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 0.95f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9f, 0.9f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.7f, 0.7f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.9f, 0.9f, 0.0f, 1.0f);
}

void Menu::Render(HWND hwnd) {
    // Start ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Always show cursor
    ImGui::GetIO().MouseDrawCursor = true;

    // Get window size
    RECT rc;
    GetClientRect(hwnd, &rc);
    ImVec2 windowSize = ImVec2((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));

    // Main window setup
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    ImGui::Begin("##MainWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse);

    // ===== TITLE BAR =====
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 0.95f));
    ImGui::BeginChild("##TitleBar", ImVec2(windowSize.x, 35), false,
        ImGuiWindowFlags_NoMove);

    // Title text
    ImGui::SetCursorPos(ImVec2(15, 10));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "ARCHER MASTER");

    // Close button
    ImGui::SameLine(windowSize.x - 90);
    ImGui::SetCursorPosY(7);

    // Style close button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    if (ImGui::Button(" X CLOSE ", ImVec2(80, 25))) {
        Settings::bShouldQuit = true;
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }

    // Check if hovering over button
    s_bIsOverButton = ImGui::IsItemHovered();

    ImGui::PopStyleColor(4);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Separator line
    ImGui::GetWindowDrawList()->AddLine(
        ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 35),
        ImVec2(ImGui::GetWindowPos().x + windowSize.x, ImGui::GetWindowPos().y + 35),
        ImGui::GetColorU32(ImVec4(0.5f, 0.5f, 0.5f, 0.5f)), 2.0f);

    // Spacing after title
    ImGui::Spacing();
    ImGui::Spacing();

    // ===== MAIN CONTENT =====
    // Features section
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "GAME FEATURES");
    ImGui::Separator();
    ImGui::Spacing();

    // Checkboxes
    ImGui::Checkbox("BUNNY HOP", feature[0]->getEnabledPtr());
    ImGui::Spacing();
    ImGui::Checkbox("AIM ASSIST", feature[2]->getEnabledPtr());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Checkbox("WALLHACK (ESP)", feature[1]->getEnabledPtr());
    ImGui::Spacing();
    ImGui::Checkbox("NO RECOIL", feature[3]->getEnabledPtr());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Info section
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "CONTROLS INFO:");
    ImGui::BulletText("Click ANYWHERE (except close button) to move");
    ImGui::BulletText("Drag to reposition window");
    ImGui::BulletText("Checkboxes work normally");
    ImGui::BulletText("Press INSERT to toggle menu");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Status info
    if (s_bIsDragging) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "STATUS: DRAGGING WINDOW");
    }
    else if (s_bIsOverButton) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "STATUS: OVER CLOSE BUTTON");
    }
    else {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 1.0f, 1.0f), "STATUS: READY TO DRAG");
    }

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    // Demo button
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.6f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.7f, 1.0f, 1.0f));

    if (ImGui::Button("TEST BUTTON (CLICK ME)", ImVec2(200, 40))) {
        MessageBoxA(hwnd, "Test button clicked!\nWindow dragging works everywhere except buttons.", "Success", MB_OK);
    }

    ImGui::PopStyleColor(3);

    // Tooltip for dragging
    if (ImGui::IsWindowHovered() && !s_bIsOverButton) {
        ImGui::SetTooltip("Click and drag to move this window!\n(Works anywhere except buttons)");
    }

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::MenuThread() {
    // Register window class
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        Menu::WndProc,
        0L,
        0L,
        GetModuleHandle(NULL),
        NULL,
        LoadCursor(NULL, IDC_ARROW),
        NULL,
        NULL,
        L"ArcherClass",
        NULL
    };
    RegisterClassEx(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED,
        wc.lpszClassName,
        L"Archer Master",
        WS_POPUP,
        100, 100,
        450, 500,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    // Transparency
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    // Remove DWM borders
    MARGINS m = { 0, 0, 0, 0 };
    DwmExtendFrameIntoClientArea(hwnd, &m);

    // DirectX setup
    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = 450;
    d3dpp.BackBufferHeight = 500;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.hDeviceWindow = hwnd;

    IDirect3DDevice9* pDevice = nullptr;
    pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice);

    Installation(hwnd, pDevice);
    ShowWindow(hwnd, SW_SHOW);

    // Main loop
    MSG msg;
    while (!Settings::bShouldQuit) {
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                Settings::bShouldQuit = true;
            }
        }

        pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                      D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

        if (pDevice->BeginScene() >= 0) {
            Render(hwnd);
            pDevice->EndScene();
        }

        pDevice->Present(NULL, NULL, NULL, NULL);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (pDevice) pDevice->Release();
    if (pD3D) pD3D->Release();

    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
}