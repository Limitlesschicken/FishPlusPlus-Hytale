/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <cstdint>
#include "../../Math/Vector2.h"

struct Character {
	uint32_t id;
	Vector2 Size;
	Vector2 Bearing;
	uint32_t Advance;
	uint32_t Ascender;
};