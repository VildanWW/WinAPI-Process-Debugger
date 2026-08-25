#pragma once
#include <Windows.h>
#include <d3d9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Settings.h"
#include <thread>
#include <dwmapi.h>
#include <vector>
#include <memory>
#include "IBaseFeature.h"
#pragma comment(lib, "dwmapi.lib")

namespace EF = ProcessDebugger::Features;

inline std::vector<std::unique_ptr<EF::IBaseFeature>> feature;

namespace Menu {
	void Installation(HWND hwnd, IDirect3DDevice9* pDevice);
	void Render(HWND hwnd);

	LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void MenuThread();
}

