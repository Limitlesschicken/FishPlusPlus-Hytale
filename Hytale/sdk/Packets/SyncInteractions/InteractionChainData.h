/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "sdk/BaseDataTypes/Array.h"
#include "sdk/BaseDataTypes/HytaleString.h"
#include "sdk/BaseDataTypes/Dictionary.h"
#include "../Structs/IndependentStructs.h"
#include "../Structs/Enums.h"
#include "core.h"

struct InteractionChainData : Object { // Struct from pEric
	Vector3f* hit_location;
	HytaleString* hit_detail;
	BlockPosition* block_position;
	Vector3f* hit_normal;
	int entity_id;
	int target_slot;
	Guuid proxy_id;

	static InteractionChainData* CreateBlockPosData(BlockPosition* blockPos) {
		InteractionChainData* data = API::RHPNewFast<InteractionChainData*>(SM::InteractionChainData_MTAddress);
		data->block_position = blockPos;
		data->entity_id = -1;
		data->target_slot = INT_MIN;
		return data;
	}

	void DBGPrint(int indent = 0) {
		std::string prefix(indent, ' ');
		Util::log("%sInteractionChainData:", prefix.c_str());
		Util::log("%s  hit_location: %s", prefix.c_str(), hit_location ? ("(" + std::to_string(hit_location->x) + ", " + std::to_string(hit_location->y) + ", " + std::to_string(hit_location->z) + ")").c_str() : "nullptr");
		Util::log("%s  hit_detail: %s", prefix.c_str(), hit_detail ? hit_detail->getString().c_str() : "nullptr");
		Util::log("%s  block_position: %s", prefix.c_str(), block_position ? ("(" + std::to_string(block_position->x) + ", " + std::to_string(block_position->y) + ", " + std::to_string(block_position->z) + ")").c_str() : "nullptr");
		Util::log("%s  hit_normal: %s", prefix.c_str(), hit_normal ? ("(" + std::to_string(hit_normal->x) + ", " + std::to_string(hit_normal->y) + ", " + std::to_string(hit_normal->z) + ")").c_str() : "nullptr");
		Util::log("%s  entity_id: %d", prefix.c_str(), entity_id);
		Util::log("%s  target_slot: %d", prefix.c_str(), target_slot);
		Util::log("%s  proxy_id: %d %d %d", prefix.c_str(), proxy_id.a, proxy_id.b, proxy_id.c);
	}
};
