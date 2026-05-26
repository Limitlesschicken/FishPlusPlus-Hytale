/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "sdk/BaseDataTypes/Array.h"
#include "sdk/BaseDataTypes/HytaleString.h"
#include "sdk/BaseDataTypes/Dictionary.h"
#include "Structs/IndependentStructs.h"
#include "Structs/Enums.h"
#include "SyncInteractions/SyncInteractionChain.h"
#include "SyncInteractions/CreateSyncData.h"
#include "core.h"



struct SyncInteractionChainsPacket : Object { // Struct from pEric
	Array<SyncInteractionChain*>* updates;

	static void SendOpenContainer(Vector3 pos) {
		Packets::SendPacketImmediate(CreateOpenContainer(pos));
	}

	static void SendBreakBlock(Vector3 pos) {
		if (!Util::IsInGame())
			return;
		Packets::SendPacketImmediate(CreateBreakBlock(pos));
		Packets::SendPacketImmediate(CreateBreakBlockLast(pos));
	}

	void DBGPrint() {
		Util::log("SyncInteractionChainsPacket:");
		Util::log("  updates: %s", updates ? ("[Array Count: " + std::to_string(updates->count) + "]").c_str() : "nullptr");
		if (updates) {
			for (int i = 0; i < updates->count; i++) {
				Util::log("  [%d]:", i);
				SyncInteractionChain* chain = updates->get(i);
				if (chain) {
					chain->DBGPrint(4);
				} else {
					Util::log("    nullptr");
				}
			}
		}
	}
};

