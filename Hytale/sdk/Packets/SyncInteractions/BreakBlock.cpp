/*
 * Copyright (c) FishPlusPlus.
 */
#include "CreateSyncData.h"
#include "../SyncInteractionChains.h"

Array<InteractionSyncData*>* CreateBreakBlockArray(InteractionModule* interaction, BlockPosition* blockPos) {
	auto blockHash = "Block_Primary"_hash;
	auto failedHash = "***Block_Break_Failed_Next_DefaultValue"_hash;
	InteractionSyncData* syncData0 = InteractionSyncData::CreateInteractionSyncData();
	syncData0->operation_counter = 0;
	syncData0->root_interaction = interaction->getInteractionID(blockHash);
	syncData0->state = kInteractionStateFinished;
	InteractionSyncData* syncData1 = InteractionSyncData::CreateInteractionSyncData();
	syncData1->operation_counter = 1;
	syncData1->root_interaction = interaction->getInteractionID(blockHash);
	syncData1->block_position = blockPos;
	syncData1->block_face = BlockFace::kBlockFaceNorth;
	syncData1->state = kInteractionStateFailed;
	InteractionSyncData* syncData2 = InteractionSyncData::CreateInteractionSyncData();
	syncData2->operation_counter = 3;
	syncData2->root_interaction = interaction->getInteractionID(blockHash);
	syncData2->state = kInteractionStateFinished;
	InteractionSyncData* syncData3 = InteractionSyncData::CreateInteractionSyncData();
	syncData3->operation_counter = 4;
	syncData3->block_position = blockPos;
	syncData3->root_interaction = interaction->getInteractionID(blockHash);
	syncData3->state = kInteractionStateFailed;

	InteractionSyncData* syncData4 = InteractionSyncData::CreateInteractionSyncData();
	syncData4->operation_counter = 6;
	syncData4->block_position = blockPos;
	syncData4->root_interaction = interaction->getInteractionID(blockHash);
	syncData4->state = kInteractionStateFinished;

	InteractionSyncData* syncData5 = InteractionSyncData::CreateInteractionSyncData();
	syncData5->operation_counter = 7;
	syncData5->root_interaction = interaction->getInteractionID(blockHash);
	syncData5->state = kInteractionStateFinished;

	InteractionSyncData* syncData6 = InteractionSyncData::CreateInteractionSyncData();
	syncData6->operation_counter = 0;
	syncData6->root_interaction = interaction->getInteractionID(failedHash);
	syncData6->state = kInteractionStateFinished;

	InteractionSyncData* syncData7 = InteractionSyncData::CreateInteractionSyncData();
	syncData7->operation_counter = 8;
	syncData7->root_interaction = interaction->getInteractionID(blockHash);
	syncData7->state = kInteractionStateFinished;

	InteractionSyncData* syncData8 = InteractionSyncData::CreateInteractionSyncData();
	syncData8->operation_counter = 9;
	syncData8->root_interaction = interaction->getInteractionID(blockHash);
	syncData8->state = kInteractionStateNotFinished;


	Array<InteractionSyncData*>* arrayData = API::RHPNewArray<Array<InteractionSyncData*>*>(SM::Array_InteractionSyncData_MTAddress, 9);


	arrayData->list[0] = syncData0;
	arrayData->list[1] = syncData1;
	arrayData->list[2] = syncData2;
	arrayData->list[3] = syncData3;
	arrayData->list[4] = syncData4;
	arrayData->list[5] = syncData5;
	arrayData->list[6] = syncData6;
	arrayData->list[7] = syncData7;
	arrayData->list[8] = syncData8;

	return arrayData;
}

SyncInteractionChainsPacket* CreateBreakBlock(Vector3 pos) {
	GameInstance* gameInstance = Util::getGameInstance();
	Entity* player = gameInstance->Player;
	InteractionModule* interactionModule = gameInstance->InteractionModule;
	BlockPosition* blockPos = BlockPosition::CreateBlockPos((int)pos.x, (int)pos.y, (int)pos.z);
	SyncInteractionChain* chain = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	chain->interaction_type = InteractionType::kInteractionTypePrimary;
	if (player->PrimaryItem && player->PrimaryItem->Id)
		chain->item_in_hand_id = player->PrimaryItem->Id;
	if (player->SecondaryItem && player->SecondaryItem->Id)
		chain->utility_item_id = player->SecondaryItem->Id;
	chain->state = kInteractionStateNotFinished; 
	chain->active_hotbar_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain->active_utility_slot = Util::getGameInstance()->InventoryModule->UtilityActiveSlot;
	chain->active_tools_slot = -1;
	chain->override_root_interaction = INT_MIN;
	chain->equip_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain->chain_id = (interactionModule->InteractionChainCounter + 1);
	chain->operation_base_index = 0;
	chain->initial = true;
	chain->data = InteractionChainData::CreateBlockPosData(blockPos);
	chain->interaction_data = CreateBreakBlockArray(interactionModule, blockPos);

	Array<SyncInteractionChain*>* updates = API::RHPNewArray<Array<SyncInteractionChain*>*>(SM::Array_SyncInteractionChain_MTAddress, 1);
	updates->list[0] = chain;

	SyncInteractionChainsPacket* packet = CreatePacket<SyncInteractionChainsPacket*>(SyncInteractionChains_BI);
	packet->updates = updates;
	return packet;
}

Array<InteractionSyncData*>* CreateBreakBlockArrayLast(InteractionModule* interaction, BlockPosition* blockPos) {
	auto blockHash = "****Block_Swing_Left_Next_Interactions_2_Interactions_0_DefaultValue"_hash;
	InteractionSyncData* syncData0 = InteractionSyncData::CreateInteractionSyncData();
	syncData0->operation_counter = 0;
	syncData0->root_interaction = interaction->getInteractionID(blockHash);
	syncData0->state = kInteractionStateFinished;
	syncData0->progress = 0.1;

	Array<InteractionSyncData*>* arrayData = API::RHPNewArray<Array<InteractionSyncData*>*>(SM::Array_InteractionSyncData_MTAddress, 1);


	arrayData->list[0] = syncData0;

	return arrayData;
}

Array<InteractionSyncData*>* CreateBreakBlockArrayLast2(InteractionModule* interaction, BlockPosition* blockPos) {
	auto blockHash = "Block_Primary"_hash;
	auto nextHash = "**Block_Swing_Left_Next_Interactions_0"_hash;
	InteractionSyncData* syncData0 = InteractionSyncData::CreateInteractionSyncData();
	syncData0->operation_counter = 0;
	syncData0->root_interaction = interaction->getInteractionID(nextHash);
	syncData0->state = kInteractionStateFinished;
	syncData0->progress = 0.1;

	InteractionSyncData* syncData1 = InteractionSyncData::CreateInteractionSyncData();
	syncData0->operation_counter = 0;
	syncData0->root_interaction = interaction->getInteractionID(blockHash);
	syncData0->state = kInteractionStateFinished;

	InteractionSyncData* syncData2 = InteractionSyncData::CreateInteractionSyncData();
	syncData0->operation_counter = 0;
	syncData0->root_interaction = interaction->getInteractionID(blockHash);
	syncData0->state = kInteractionStateFinished;

	Array<InteractionSyncData*>* arrayData = API::RHPNewArray<Array<InteractionSyncData*>*>(SM::Array_InteractionSyncData_MTAddress, 3);


	arrayData->list[0] = syncData0;
	arrayData->list[1] = syncData1;
	arrayData->list[2] = syncData2;

	return arrayData;
}

SyncInteractionChainsPacket* CreateBreakBlockLast(Vector3 pos) {
	GameInstance* gameInstance = Util::getGameInstance();
	Entity* player = gameInstance->Player;
	InteractionModule* interactionModule = gameInstance->InteractionModule;
	BlockPosition* blockPos = BlockPosition::CreateBlockPos((int)pos.x, (int)pos.y, (int)pos.z);
	SyncInteractionChain* chain = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	chain->interaction_type = InteractionType::kInteractionTypePrimary;
	if (player->PrimaryItem && player->PrimaryItem->Id)
		chain->item_in_hand_id = player->PrimaryItem->Id;
	if (player->SecondaryItem && player->SecondaryItem->Id)
		chain->utility_item_id = player->SecondaryItem->Id;
	chain->state = kInteractionStateFinished;
	chain->active_hotbar_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain->active_utility_slot = Util::getGameInstance()->InventoryModule->UtilityActiveSlot;
	chain->active_tools_slot = -1;
	chain->override_root_interaction = INT_MIN;
	chain->equip_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain->chain_id = (interactionModule->InteractionChainCounter + 1);
	chain->operation_base_index = 1;
	chain->initial = false;
	chain->data = InteractionChainData::CreateBlockPosData(blockPos);
	chain->interaction_data = CreateBreakBlockArrayLast(interactionModule, blockPos);

	SyncInteractionChain* chain2 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	chain2->interaction_type = InteractionType::kInteractionTypePrimary;
	if (player->PrimaryItem && player->PrimaryItem->Id)
		chain2->item_in_hand_id = player->PrimaryItem->Id;
	if (player->SecondaryItem && player->SecondaryItem->Id)
		chain2->utility_item_id = player->SecondaryItem->Id;
	chain2->state = kInteractionStateFinished;
	chain2->active_hotbar_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain2->active_utility_slot = Util::getGameInstance()->InventoryModule->UtilityActiveSlot;
	chain2->active_tools_slot = -1;
	chain2->override_root_interaction = INT_MIN;
	chain2->equip_slot = Util::getGameInstance()->InventoryModule->HotbarActiveSlot;
	chain2->chain_id = (interactionModule->InteractionChainCounter + 1);
	chain2->operation_base_index = 1;
	chain2->initial = false;
	chain2->data = InteractionChainData::CreateBlockPosData(blockPos);
	chain2->interaction_data = CreateBreakBlockArrayLast2(interactionModule, blockPos);

	Array<SyncInteractionChain*>* updates = API::RHPNewArray<Array<SyncInteractionChain*>*>(SM::Array_SyncInteractionChain_MTAddress, 2);
	updates->list[0] = chain;
	updates->list[1] = chain2;

	SyncInteractionChainsPacket* packet = CreatePacket<SyncInteractionChainsPacket*>(SyncInteractionChains_BI);
	packet->updates = updates;
	interactionModule->InteractionChainCounter++;

	SyncInteractionChain* chain3 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain4 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain5 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain6 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain7 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain8 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain9 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain10 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain11 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	SyncInteractionChain* chain12 = API::RHPNewFast<SyncInteractionChain*>(SM::SyncInteractionChain_MTAddress);
	return packet;
}