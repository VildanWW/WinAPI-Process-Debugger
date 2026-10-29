#pragma once
#include <Windows.h>

namespace ProcessDebugger {
	namespace ProjectMath {
		inline constexpr float PI = 3.14159265358979323846f;

		struct Vector3 { float x, y, z; };
		struct ViewMatrix { float m[16]; };

		bool WorldToScreen(const Vector3& pos, const ViewMatrix& viewMatrix, POINT& out, int w, int h, float& dist);
	}
}


