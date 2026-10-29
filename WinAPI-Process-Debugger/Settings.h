#pragma once
#include <atomic>

namespace ProcessDebugger {
	namespace Settings {
		inline std::atomic<bool> running{ true };

		inline const char* nameProcess = "Counter-Strike";

		inline const wchar_t* nameClient = L"client.dll";
		inline const wchar_t* nameHw = L"hw.dll";

		inline int bhop = VK_SPACE;
		inline int clickAim = VK_LBUTTON;
	}
}
