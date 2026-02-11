#pragma once

#include "GameInstance.h"
#include "ClientMovementStates.h"

class DefaultMovementController
{
public:
	char pad_0000[20]; //0x0000
	float N00000862; //0x0014
	GameInstance* GameInstance; //0x0018
	char pad_0020[40]; //0x0020
	float SpeedMultiplier; //0x0048
	char pad_004C[88]; //0x004C
	class ClientMovementStates clientMovementStates; //0x00A4
	char pad_00B7[41]; //0x00B7
	Vector3 Velocity; //0x00E0
	Vector3 WishDirection; //0x00EC
	char pad_00F8[84]; //0x00F8
	float N00000880; //0x014C
	char pad_0150[35]; //0x0150
	bool N00000981; //0x0173
	char pad_0174[3]; //0x0174
	bool IsOnGround; //0x0177
	char pad_0178[2772]; //0x0178

}; //Size: 0x044C4C