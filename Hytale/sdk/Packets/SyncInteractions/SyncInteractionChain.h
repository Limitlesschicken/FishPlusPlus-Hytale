/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "sdk/BaseDataTypes/Array.h"
#include "sdk/BaseDataTypes/HytaleString.h"
#include "sdk/BaseDataTypes/Dictionary.h"
#include "../Structs/IndependentStructs.h"
#include "../Structs/Enums.h"
#include "InteractionChainData.h"
#include "InteractionSyncData.h"
#include "core.h"

struct ForkedChainId : Object { // Struct from pEric
	ForkedChainId* forked_id;
	int entry_index;
	int sub_index;

	void DBGPrint(int indent = 0) {
		std::string prefix(indent, ' ');
		Util::log("%sForkedChainId:", prefix.c_str());
		Util::log("%s  entry_index: %d", prefix.c_str(), entry_index);
		Util::log("%s  sub_index: %d", prefix.c_str(), sub_index);
		if (forked_id) {
			Util::log("%s  forked_id (nested):", prefix.c_str());
			forked_id->DBGPrint(indent + 4);
		}
		else {
			Util::log("%s  forked_id: nullptr", prefix.c_str());
		}
	}
};

struct SyncInteractionChain : Object { // Struct from pEric
	HytaleString* item_in_hand_id;
	HytaleString* utility_item_id;
	HytaleString* tools_item_id;
	ForkedChainId* forked_id;
	InteractionChainData* data;
	Array<SyncInteractionChain*>* new_forks;
	Array<InteractionSyncData*>* interaction_data;
	int active_hotbar_slot;
	int active_utility_slot;
	int active_tools_slot;
	int override_root_interaction;
	int equip_slot;
	int chain_id;
	int operation_base_index;
	bool initial;
	bool desync;
	InteractionType interaction_type;
	InteractionState state;

	void DBGPrint(int indent = 0) {
		std::string prefix(indent, ' ');
		Util::log("%sSyncInteractionChain:", prefix.c_str());
		Util::log("%s  interaction_type: %d", prefix.c_str(), interaction_type);
		Util::log("%s  state: %d", prefix.c_str(), state);
		Util::log("%s  item_in_hand_id: %s", prefix.c_str(), item_in_hand_id ? item_in_hand_id->getString().c_str() : "nullptr");
		Util::log("%s  utility_item_id: %s", prefix.c_str(), utility_item_id ? utility_item_id->getString().c_str() : "nullptr");
		Util::log("%s  tools_item_id: %s", prefix.c_str(), tools_item_id ? tools_item_id->getString().c_str() : "nullptr");
		Util::log("%s  active_hotbar_slot: %d", prefix.c_str(), active_hotbar_slot);
		Util::log("%s  active_utility_slot: %d", prefix.c_str(), active_utility_slot);
		Util::log("%s  active_tools_slot: %d", prefix.c_str(), active_tools_slot);
		Util::log("%s  override_root_interaction: %d", prefix.c_str(), override_root_interaction);
		Util::log("%s  equip_slot: %d", prefix.c_str(), equip_slot);
		Util::log("%s  chain_id: %d", prefix.c_str(), chain_id);
		Util::log("%s  operation_base_index: %d", prefix.c_str(), operation_base_index);
		Util::log("%s  initial: %s", prefix.c_str(), initial ? "true" : "false");
		Util::log("%s  desync: %s", prefix.c_str(), desync ? "true" : "false");

		if (forked_id) {
			Util::log("%s  forked_id:", prefix.c_str());
			forked_id->DBGPrint(indent + 4);
		}
		else {
			Util::log("%s  forked_id: nullptr", prefix.c_str());
		}

		if (data) {
			Util::log("%s  data:", prefix.c_str());
			data->DBGPrint(indent + 4);
		}
		else {
			Util::log("%s  data: nullptr", prefix.c_str());
		}

		Util::log("%s  new_forks: %s", prefix.c_str(), new_forks ? ("[Array Count: " + std::to_string(new_forks->count) + "]").c_str() : "nullptr");
		if (new_forks) {
			for (int i = 0; i < new_forks->count; i++) {
				Util::log("%s    [%d]:", prefix.c_str(), i);
				new_forks->get(i)->DBGPrint(indent + 6);
			}
		}

		Util::log("%s  interaction_data: %s", prefix.c_str(), interaction_data ? ("[Array Count: " + std::to_string(interaction_data->count) + "]").c_str() : "nullptr");
		if (interaction_data) {
			for (int i = 0; i < interaction_data->count; i++) {
				Util::log("%s    [%d]:", prefix.c_str(), i);
				interaction_data->get(i)->DBGPrint(indent + 6);
			}
		}
	}
};
