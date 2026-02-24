/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <cstdint>
#include "../../Math/Vector2.h"

struct Character {
	Vector2 Size;
	Vector2 Bearing;
	uint32_t Advance;
	uint32_t Ascender;
	float tx;
	float tw;
};