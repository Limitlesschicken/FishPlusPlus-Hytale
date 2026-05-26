/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "sdk/BaseDataTypes/Object.h"
#include "Structs/IndependentStructs.h"

#pragma pack(push, 1)
struct ClientPlaceBlockPacketBuffer {
	byte flagOfSomeSort;
	int posX;
	int posY;
	int posZ;
	Rotation rotationYaw;
	Rotation rotationPitch;
	Rotation rotationRoll;
	int placedBlockId;
	bool quickReplace;
	bool noPhysics;
};
#pragma pack(pop)


struct ClientPlaceBlockPacket : Object { // Struct from pEric
	BlockPosition* position;
	BlockRotation* rotation;
	int placedBlockId;
	bool quickReplace;
	bool noPhysics;

	static void Send(Vector3 pos, int placedBlockId = 0, bool quickReplace = false, bool noPhysics = false) {
		ClientPlaceBlockPacket* packet = CreatePacket<ClientPlaceBlockPacket*>(ClientPlaceBlock_C2S);
		BlockPosition* position = new BlockPosition(pos.toFloor());
		BlockRotation* rotation = new BlockRotation();
		packet->position = position;
		packet->rotation = rotation;
		packet->placedBlockId = placedBlockId;
		packet->quickReplace = quickReplace;
		packet->noPhysics = noPhysics;

		Packets::SendPacketImmediate(packet);

		delete position;
		delete rotation;
	}
};