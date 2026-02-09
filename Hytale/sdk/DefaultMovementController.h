#pragma once

#include "GameInstance.h"
#include "ClientMovementStates.h"

class DefaultMovementController
{
public:
	char pad_0000[20]; //0x0000
	float N00000862; //0x0014
	GameInstance* GameInstance; //0x0018
	char pad_0020[132]; //0x0020
	ClientMovementStates clientMovementStates; //0x00A4
	char pad_00B7[149]; //0x00B7
	float N00000880; //0x014C
	char pad_0150[764]; //0x0150
}; //Size: 0x044C4C