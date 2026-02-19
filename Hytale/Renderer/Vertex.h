/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Math/Vector3.h"
#include "../Math/Color.h"

struct Vertex {
	Vector3 position;
	Color color;

	Vertex(Vector3 position, Color color) {
		this->position = position;
		this->color = color;
	}
};