/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>
#include <Windows.h>

#include "../Math/Vector3.h"
#include "../Math/BoundingBox.h"

#include "HytaleString.h"
#include "Array.h"



struct EntityAssetStruct {
	char pad[0x8];
	HytaleString* entityString;
};

class Entity
{
public:

	enum EntityPlayerType {
		Other = -1,
		Player = 1
	};
	enum EntityType {
		None,
		Character,
		Item,
		Block
	};

	char pad_0000[24]; //0x0000
	void* GameInstance; //0x0018
	char pad_0020[32]; //0x0020
	bool N0000077D; //0x0040
	char pad_0041[87]; //0x0041
	EntityAssetStruct* AssetNameStruct; //0x0098
	char pad_00A0[96]; //0x00A0
	HytaleString* Name; //0x0100
	char pad_0108[192]; //0x0108
	EntityPlayerType entityPlayerType; //0x01C8
	char pad_01CC[8]; //0x01CC
	EntityType entityType; // 0x01D4
	char pad_1D8[28]; //0x01D8
	float Scale; //0x01F4
	char pad_01F8[78]; //0x01F8
	bool wasOnGround; //0x0246
	bool wasInFluid; //0x0247
	bool wasFalling; //0x0248
	bool wasJunping; //0x0249
	bool usable; //0x024A
	char pad_024B[33]; //0x024B
	Vector3 OldPos; //0x0264
	Vector3 NextPos; //0x0270
	Vector3 Position; //0x027C
	Vector3 RenderPos; //0x0288
	BoundingBox Hitbox; //0x0294
	BoundingBox DefaultHitbox;
	BoundingBox CrouchedHitbox;
	BoundingBox IDKDefaultHitbox;
	BoundingBox IDKCrouchedHitbox;
	char pad_0314[200]; //0x0314
	float pitchRad; //0x03DC
	float yawRad; //0x03E0
	char pad_03E4[4]; //0x03E4
	float pitchRadOld; //0x03E8
	float yawRadOld; //0x03EC
	char pad_0388[1268]; //0x0388

}; //Size: 0x087C