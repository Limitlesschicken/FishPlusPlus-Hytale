/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

class Time
{
public:
	char pad_0000[32]; //0x0000
	float WorldTime; //0x0020
	char pad_0024[1052]; //0x0024
}; //Size: 0x0440