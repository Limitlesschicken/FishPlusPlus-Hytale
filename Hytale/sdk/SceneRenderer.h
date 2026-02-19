/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Math/Matrix4x4.h"

struct SceneRenderer {
	char pad[0x300];
	Matrix4x4 MPV;
};