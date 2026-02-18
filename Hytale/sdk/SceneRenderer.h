#pragma once

#include "Math/Matrix4x4.h"

struct SceneRenderer {
	char pad[0x300];
	Matrix4x4 MPV;
};