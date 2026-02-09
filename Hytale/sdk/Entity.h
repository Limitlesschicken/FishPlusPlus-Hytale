#pragma once

#include <string>
#include <Windows.h>

#include "../Math/Vector3.h"
#include "../Math/BoundingBox.h"

#include "HytaleString.h"



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
	char pad_0108[204];
	EntityType entityType;
	char pad_1D8[28];
	float Scale; //0x01F4
	char pad_01F8[44]; //0x01F8
	EntityPlayerType entityPlayerType; //0x0224
	char pad_0228[16]; //0x0228
	float DisplayHealth; //0x0238
	char pad_023C[10]; //0x023C
	bool wasOnGround; //0x0246
	bool wasInFluid; //0x0247
	bool wasFalling; //0x0248
	bool wasJunping; //0x0249
	bool usable; //0x024A
	char pad_024B[25]; //0x024B
	Vector3 OldPos; //0x0264
	Vector3 NextPos; //0x0270
	Vector3 Position; //0x027C
	Vector3 RenderPos; //0x0288
	BoundingBox Hitbox; //0x0294
	BoundingBox DefaultHitbox;
	BoundingBox CrouchedHitbox;
	BoundingBox IDKDefaultHitbox;
	BoundingBox IDKCrouchedHitbox;
	char pad_030C[104]; //0x030C
	float pitchRad; //0x0374
	float yawRad; //0x0378
	char pad_037C[4]; //0x037C
	float pitchRadOld; //0x0380
	float yawRadOld; //0x0384
	char pad_0388[1268]; //0x0388

}; //Size: 0x087C