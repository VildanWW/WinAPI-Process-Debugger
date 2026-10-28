#include "ProjectMath.h"
#include "GameStructures.h"

namespace ProcessDebugger {
    namespace ProjectMath {
		bool WorldToScreen(const Vector3& pos, const ViewMatrix& viewMatrix, POINT& out, int w, int h, float& dist) {
			float clipW = pos.x * viewMatrix.m[3] + pos.y * viewMatrix.m[7] + pos.z * viewMatrix.m[11] + viewMatrix.m[15];
			dist = clipW;
			if (clipW < 0.1f) return false;

			float x = (pos.x * viewMatrix.m[0] + pos.y * viewMatrix.m[4] + pos.z * viewMatrix.m[8] + viewMatrix.m[12]) / clipW;
			float y = (pos.x * viewMatrix.m[1] + pos.y * viewMatrix.m[5] + pos.z * viewMatrix.m[9] + viewMatrix.m[13]) / clipW;

			out.x = (int)((w / 2.0f) * (1 + x));
			out.y = (int)((h / 2.0f) * (1 - y));
			return true;
		}
	}
}