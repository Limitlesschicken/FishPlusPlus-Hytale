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

struct SelectedHitEntity : Object { // Struct from pEric
	Vector3f* hit_location;
	Position* position;
	Direction* body_rotation;
	int network_id;

	void DBGPrint(int indent = 0) {
		std::string prefix(indent, ' ');
		Util::log("%sSelectedHitEntity:", prefix.c_str());
		Util::log("%s  hit_location: %s", prefix.c_str(), hit_location ? "(" + std::to_string(hit_location->x) + ", " + std::to_string(hit_location->y) + ", " + std::to_string(hit_location->z) + ")" : "nullptr");
		Util::log("%s  position: %s", prefix.c_str(), position ? "(" + std::to_string(position->x) + ", " + std::to_string(position->y) + ", " + std::to_string(position->z) + ")" : "nullptr");
		Util::log("%s  body_rotation: %s", prefix.c_str(), body_rotation ? "(" + std::to_string(body_rotation->pitch) + ", " + std::to_string(body_rotation->yaw) + ")" : "nullptr");
		Util::log("%s  network_id: %d", prefix.c_str(), network_id);
	}
};

struct InteractionSyncData : Object { // Struct from pEric
	BlockPosition* block_position;
	BlockRotation* block_rotation;
	Dictionary<InteractionType, int>* fork_counts;
	Array<SelectedHitEntity*>* hit_entities;
	Position* attacker_pos;
	Direction* attacker_rot;
	Position* raycast_hit;
	Vector3f* raycast_normal;
	float progress;
	int operation_counter;
	int root_interaction;
	int total_forks;
	int entity_id;
	int entered_root_interaction;
	int placed_block_id;
	float charge_value;
	int chaining_index;
	int flag_index;
	float raycast_distance;
	int next_label;
	InteractionState state;
	BlockFace block_face;
	MovementDirection movement_direction;
	ApplyForceState apply_force_state;
	Guuid generated_u_u_i_d;

	static InteractionSyncData* CreateInteractionSyncData() {
		InteractionSyncData* syncData = API::RHPNewFast<InteractionSyncData*>(SM::InteractionSyncData_MTAddress);
		syncData->entered_root_interaction = INT_MIN;
		syncData->placed_block_id = INT_MIN;
		syncData->charge_value = -1.0f;
		syncData->chaining_index = -1;
		syncData->flag_index = -1;
		return syncData;
	}

	void DBGPrint(int indent = 0) {
		std::string prefix(indent, ' ');
		Util::log("%sInteractionSyncData:", prefix.c_str());
		Util::log("%s  block_position: %s", prefix.c_str(), block_position ? ("(" + std::to_string(block_position->x) + ", " + std::to_string(block_position->y) + ", " + std::to_string(block_position->z) + ")").c_str() : "nullptr");
		Util::log("%s  block_rotation: %s", prefix.c_str(), block_rotation ? "[BlockRotation]" : "nullptr");
		Util::log("%s  fork_counts: %s", prefix.c_str(), fork_counts ? ("[Dictionary Count: " + std::to_string(fork_counts->count) + "]").c_str() : "nullptr");
		Util::log("%s  hit_entities: %s", prefix.c_str(), hit_entities ? ("[Array Count: " + std::to_string(hit_entities->count) + "]").c_str() : "nullptr");
		if (hit_entities) {
			for (int i = 0; i < hit_entities->count; i++) {
				Util::log("%s    [%d]:", prefix.c_str(), i);
				hit_entities->get(i)->DBGPrint(indent + 6);
			}
		}
		Util::log("%s  attacker_pos: %s", prefix.c_str(), attacker_pos ? ("(" + std::to_string(attacker_pos->x) + ", " + std::to_string(attacker_pos->y) + ", " + std::to_string(attacker_pos->z) + ")").c_str() : "nullptr");
		Util::log("%s  attacker_rot: %s", prefix.c_str(), attacker_rot ? ("(" + std::to_string(attacker_rot->pitch) + ", " + std::to_string(attacker_rot->yaw) + ")").c_str() : "nullptr");
		Util::log("%s  raycast_hit: %s", prefix.c_str(), raycast_hit ? ("(" + std::to_string(raycast_hit->x) + ", " + std::to_string(raycast_hit->y) + ", " + std::to_string(raycast_hit->z) + ")").c_str() : "nullptr");
		Util::log("%s  raycast_normal: %s", prefix.c_str(), raycast_normal ? ("(" + std::to_string(raycast_normal->x) + ", " + std::to_string(raycast_normal->y) + ", " + std::to_string(raycast_normal->z) + ")").c_str() : "nullptr");
		Util::log("%s  progress: %f", prefix.c_str(), progress);
		Util::log("%s  operation_counter: %d", prefix.c_str(), operation_counter);
		if (Util::IsInGame())
			Util::log("%s  root_interaction: %s", prefix.c_str(), Util::getGameInstance()->InteractionModule->getInteractionIdName(root_interaction).c_str());
		else
			Util::log("%s  root_interaction: %d", prefix.c_str(), root_interaction);
		Util::log("%s  total_forks: %d", prefix.c_str(), total_forks);
		Util::log("%s  entity_id: %d", prefix.c_str(), entity_id);
		Util::log("%s  entered_root_interaction: %d", prefix.c_str(), entered_root_interaction);
		Util::log("%s  placed_block_id: %d", prefix.c_str(), placed_block_id);
		Util::log("%s  charge_value: %f", prefix.c_str(), charge_value);
		Util::log("%s  chaining_index: %d", prefix.c_str(), chaining_index);
		Util::log("%s  flag_index: %d", prefix.c_str(), flag_index);
		Util::log("%s  raycast_distance: %f", prefix.c_str(), raycast_distance);
		Util::log("%s  next_label: %d", prefix.c_str(), next_label);
		Util::log("%s  state: %d", prefix.c_str(), state);
		Util::log("%s  block_face: %d", prefix.c_str(), block_face);
		Util::log("%s  movement_direction: %d", prefix.c_str(), movement_direction);
		Util::log("%s  apply_force_state: %d", prefix.c_str(), apply_force_state);
		Util::log("%s  generated_u_u_i_d: %d %d %d", prefix.c_str(), generated_u_u_i_d.a, generated_u_u_i_d.b, generated_u_u_i_d.c);
	}
};