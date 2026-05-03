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
};
#pragma pack(pop)


struct ClientPlaceBlockPacket : Object { // Struct from pEric
	BlockPosition* position;
	BlockRotation* rotation;
	int placedBlockId;
	bool quickReplace;

	static void Send(Vector3 pos, int placedBlockId = 0, bool quickReplace = false) {
		ClientPlaceBlockPacket* packet = CreatePacket<ClientPlaceBlockPacket*>(ClientPlaceBlock_C2S);
		BlockPosition position = BlockPosition(pos.toFloor());
		BlockRotation rotation = BlockRotation();
		packet->position = &position;
		packet->rotation = &rotation;
		packet->placedBlockId = placedBlockId;
		packet->quickReplace = quickReplace;

		Packets::SendPacketImmediate(packet);
	}
};