/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Math/Vector3.h"


class Camera
{
public:
	char pad_0000[204]; //0x0000
	Vector3 Position; //0x00CC
	char pad_00D8[872]; //0x00D8
}; //Size: 0x0440

class StructBeforeCam
{
public:
	char pad_0000[72]; //0x0000
	Camera* Camera; //0x0048
};