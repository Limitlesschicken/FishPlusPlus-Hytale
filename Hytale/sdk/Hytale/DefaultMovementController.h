/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "ClientMovementStates.h"
#include "Math/Vector3.h"

class GameInstance;

class DefaultMovementController
{
public:
	char pad_0000[10]; //0x0000
	GameInstance* GameInstance; //0x0010
	char pad_0018[212]; //0x0018
	ClientMovementStates clientMovementStates; //0x00EC
	char pad_0100[28]; //0x0100
	Vector3 Velocity; //0x00E4
}; //Size: 0x044C4C