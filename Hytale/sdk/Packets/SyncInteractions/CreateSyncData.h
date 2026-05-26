/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "SyncInteractionChain.h"
#include "InteractionChainData.h"
#include "InteractionSyncData.h"
#include "SyncInteractionChain.h"
class SyncInteractionChainsPacket;

Array<InteractionSyncData*>* CreateOpenContainerArray(InteractionModule* interaction, BlockPosition* blockPos);
SyncInteractionChainsPacket* CreateOpenContainer(Vector3 pos);

Array<InteractionSyncData*>* CreateBreakBlockArray(InteractionModule* interaction, BlockPosition* blockPos);
SyncInteractionChainsPacket* CreateBreakBlock(Vector3 pos);
SyncInteractionChainsPacket* CreateBreakBlockLast(Vector3 pos);