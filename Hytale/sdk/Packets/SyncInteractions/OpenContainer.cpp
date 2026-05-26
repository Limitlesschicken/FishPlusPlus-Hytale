/*
 * Copyright (c) FishPlusPlus.
 */
#include "CreateSyncData.h"
#include "../SyncInteractionChains.h"

Array<InteractionSyncData*>* CreateOpenContainerArray(InteractionModule* interaction, BlockPosition* blockPos) {
	auto openContainerHash = "Open_Container"_hash;
	auto emptyUseHash = "*Empty_Interactions_Use"_hash;
	InteractionSyncData* syncData0 = InteractionSyncData::CreateInteractionSyncData();
	syncData0->operation_counter = 0;
	syncData0->root_interaction = interaction->getInteractionID(emptyUseHash);
	syncData0->state = kInteractionStateFinished;
	InteractionSyncData* syncData1 = InteractionSyncData::CreateInteractionSyncData();
	syncData1->operation_counter = 1;
	syncData1->root_interaction = interaction->getInteractionID(emptyUseHash);
	syncData1->block_position = blockPos;
	syncData1->block_face = BlockFace::kBlockFaceNorth;
	syncData1->state = kInteractionStateFinished;
	InteractionSyncData* syncData2 = InteractionSyncData::CreateInteractionSyncData();
	syncData2->operation_counter = 0;
	syncData2->root_interaction = interaction->getInteractionID(openContainerHash);
	syncData2->block_position = blockPos;
	syncData2->block_face = BlockFace::kBlockFaceNorth;
	syncData2->state = kInteractionStateFinished;
	InteractionSyncData* syncData3 = InteractionSyncData::CreateInteractionSyncData();
	syncData3->operation_counter = 2;
	syncData3->root_interaction = interaction->getInteractionID(emptyUseHash);
	Array<InteractionSyncData*>* arrayData = API::RHPNewArray<Array<InteractionSyncData*>*>(SM::Array_InteractionSyncData_MTAddress, 4);
	arrayData->list[0] = syncData0;
	arrayData->list[1] = syncData1;
	arrayData->list[2] = syncData2;
	arrayData->list[3] = syncData3;

	return arrayData;
}

SyncInteractionChainsPacket* CreateOpenContainer(Vector3 pos) {
	GameInstance* gameInstance = Util::getGameInstance();
	Entity* player = gameInstance->Player;
	InteractionModule* interactionModule = gameInstance->InteractionModule;
	BlockPosition* blockPos = BlockPosition::CreateBlockPos((int)pos.x, (int)pos.y, (int)pos.z);
	SyncInteractionChain* chain = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	chain->interaction_type = InteractionType::kInteractionTypeUse;
	if (player->PrimaryItem && player->PrimaryItem->Id)
		chain->item_in_hand_id = player->PrimaryItem->Id;
	if (player->SecondaryItem && player->SecondaryItem->Id)
		chain->utility_item_id = player->SecondaryItem->Id;
	chain->active_hotbar_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain->active_utility_slot = Util::getGameInstance()->InventoryModule->UtilityActiveSlot;
	chain->active_tools_slot = -1;
	chain->override_root_interaction = INT_MIN;
	chain->equip_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain->chain_id = (interactionModule->InteractionChainCounter + 1);
	chain->operation_base_index = 0;
	chain->initial = true;
	chain->data = InteractionChainData::CreateBlockPosData(blockPos);
	chain->interaction_data = CreateOpenContainerArray(interactionModule, blockPos);

	Array<SyncInteractionChain*>* updates = API::RHPNewArray<Array<SyncInteractionChain*>*>(SM::Array_SyncInteractionChain_MTAddress, 1);
	updates->list[0] = chain;

	SyncInteractionChainsPacket* packet = CreatePacket<SyncInteractionChainsPacket*>(SyncInteractionChains_BI);
	packet->updates = updates;
	interactionModule->InteractionChainCounter++;
	return packet;
}