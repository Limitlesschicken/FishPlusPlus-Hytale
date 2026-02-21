/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "ClientMovementStates.h"
#include "Math/Vector3.h"

class DefaultMovementController
{
public:
	char pad_0000[20]; //0x0000
	float N00000862; //0x0014
	void* GameInstance; //0x0018
	char pad_0020[40]; //0x0020
	float SpeedMultiplier; //0x0048
	char pad_004C[92]; //0x004C
	ClientMovementStates clientMovementStates; //0x00A4
	char pad_00BB[41]; //0x00BB
	Vector3 Velocity; //0x00E4
}; //Size: 0x044C4C