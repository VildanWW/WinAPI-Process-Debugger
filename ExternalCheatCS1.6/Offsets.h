#pragma once
#include <Windows.h>

namespace ProcessDebugger {
	namespace Offsets {
        inline constexpr uintptr_t positionPlayers = 0x2F3DB44;
        inline constexpr uintptr_t statePlayers = 0x3EF8CC;
        inline constexpr uintptr_t teamStatePlayers = 0x2143DC;
        inline constexpr uintptr_t myState = 0x2ED09E4;
        inline constexpr uintptr_t viewMatrix = 0x73BB90;
        inline constexpr uintptr_t viewAngles = 0x2DC2B20;
        inline constexpr uintptr_t punch = 0x2F324F0;
        inline constexpr uintptr_t responseJump = 0x1A0EA8;
        inline constexpr uintptr_t flagJump = 0x1E94C0;
	}
}