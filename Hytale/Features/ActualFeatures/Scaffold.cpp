/*
 * Copyright (c) FishPlusPlus.
 */
#include "Scaffold.h"
#include "core.h"
#include "sdk/Packets/ClientBlockPlace.h"
#include "sdk/Packets/SyncInteractionChains.h"

Scaffold::Scaffold() : Feature("Scaffold") {

}

void Scaffold::OnMoveCycle(DefaultMovementController* dmc, Vector3& offset) {
	GameInstance* gameInstance = Util::getGameInstance();
	Entity* localPlayer = gameInstance->Player;
	if (!localPlayer || !localPlayer->PrimaryItem || !localPlayer->PrimaryItem->IsBlock())
		return;

	MapModule* mapModule = gameInstance->MapModule;
	Vector3 feetPos = localPlayer->Position.toFloor().add(0, -1, 0);
	int blockID = mapModule->GetBlockID((int) feetPos.x, (int) feetPos.y, (int) feetPos.z, 0);
	if (dmc->clientMovementStates.IsCrouching) {
		if (blockID == 0 || blockID == 1) {
			feetPos.y -= 1;
			int blockIDCrouched = mapModule->GetBlockID((int) feetPos.x, (int) feetPos.y, (int) feetPos.z, 0);
			if (blockIDCrouched == 0 || blockIDCrouched == 1) {
				mapModule->SetClientBlock(feetPos, localPlayer->PrimaryItem->BlockId);
				ClientPlaceBlockPacket::Send(feetPos, localPlayer->PrimaryItem->BlockId);
			}
		}
	} else {
		if (blockID == 0 || blockID == 1) {
			mapModule->SetClientBlock(feetPos, localPlayer->PrimaryItem->BlockId);
			ClientPlaceBlockPacket::Send(feetPos, localPlayer->PrimaryItem->BlockId);
		}
	}
}

void Scaffold::Initialize() {
	RegisterEvent(this);
	Util::log("Initialized Scaffold feature");
}