#pragma once
#include "Util/Hash.h"
#include "BlockPlacementPreview.h"
#include "ClientRootInteraction.h"
#include "RaycastHit.h"

struct InteractionModule {
	char pad_0[0x10];                                   //0x0
	GameInstance* gameInstance;                         //0x10
	uint64_t field_0x10;                                //0x18
	BlockPlacementPreview* BlockPreview; 				//0x20 HytaleClient.InGame.Modules.Interaction.BlockPlacementPreview
	void* EntityPreview; 								//0x28 HytaleClient.InGame.Modules.Interaction.EntityPreview
	void* _blockOutlineRenderer; 						//0x30 HytaleClient.Graphics.Gizmos.BlockOutlineRenderer
	void* _targetBlockRaycastOptions; 					//0x38 HytaleClient.InGame.HitDetection+RaycastOptions
	Array<ClientRootInteraction*>* ClientRootInteractions; //0x40 HytaleClient.Data.ClientInteraction.ClientRootInteraction[]
	Array<void*>* ClientInteractions; 					//0x48 HytaleClient.Data.ClientInteraction.ClientInteraction[]
	void* unknown_0x50; 								//0x50 System.Collections.Generic.Dictionary`2[System.String,HytaleClient.InGame.Modules.Interaction.Cooldown
	void* unknown_0x58; 								//0x58 System.Collections.Generic.Dictionary`2[System.Int32,HytaleClient.InGame.Modules.Interaction.InteractionChain]
	void* unknown_0x60; 								//0x60 System.Collections.Generic.List`1[System.String]
	void* unknown_0x68; 								//0x68 System.Collections.Generic.List`1[System.Int32]
	void* unknown_0x70; 								//0x68 System.Collections.Generic.List`1[System.Int32]
	void* Random; 										//0x78 System.Random
	void* DamageInfos; 									//0x80 System.Collections.Generic.List`1[Hytale.Protocol.Packets.Player.DamageInfo]
	void* InteractionSyncDatas; 						//0x88 System.Collections.Generic.List`1[Hytale.Protocol.InteractionSyncData]
	void* unknown_0x90; 								//0x90 System.Single[]
	void* ClickQueueDatas;								//0x98 HytaleClient.InGame.Modules.Interaction.InteractionModule+ClickQueueData[]
	void* unknown_0xA0; 								//0xA0 System.Boolean[]
	void* unknown_0xA8; 								//0xA8 System.Boolean[]
	void* unknown_0xB0; 								//0xB0 System.Int32[]
	void* unknown_0xB8; 								//0xB8 System.String[]
	void* unknown_0xC0; 								//0xC0 HytaleClient.InGame.Modules.InventorySectionType[]
	void* unknown_0xC8; 								//0xC8 System.Collections.Generic.List`1[Hytale.Protocol.Packets.Interaction.SyncInteractionChain]
	char pad_0xD0[0x18]; 								//0xD0
	int InteractionChainCounter; 						//0xE8
	char pad_0xEC[0x4]; 								//0xEC
	bool lookingAtBlock; 								//0xF0
	char pad_0xF1[0x7]; 								//0xF1
	void* unknown_0xF8; 								//0xF8
	HytaleString* TargetedBlockName; 					//0x100
	HytaleString* TargetedBlockHint; 					//0x108
	int unknown_0x110; 									//0x110 
	char pad_0x114[0x4]; 								//0x114
	RaycastHit TargetedBlockRaycastHit; 				//0x118

	int getInteractionID(uint64_t hashedName) {
		for (int i = 0; i < ClientRootInteractions->count; i++) {
			ClientRootInteraction* interaction = ClientRootInteractions->get(i);
			if (!interaction || !interaction->ID)
				continue;
			std::string interactionName = interaction->ID->getString();
			uint64_t interactionHash = Hash::Hash(interactionName);
			if (interactionHash == hashedName)
				return interaction->interactionID;
		}
	}
};