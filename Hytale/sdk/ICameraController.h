/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Math/Vector3.h"

struct ICameraController {
	char pad_0000[32]; //0x0000
	Vector3 Position; //0x0020
	char pad_002C[1044]; //0x002C

};