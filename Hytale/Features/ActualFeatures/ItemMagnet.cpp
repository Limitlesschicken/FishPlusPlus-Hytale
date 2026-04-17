/*
 * Copyright (c) FishPlusPlus.
 */
#include "ItemMagnet.h"

#include "sdk/Hytale/Entity.h"

void ItemMagnet::OnFrame() {
	if (Util::app->Stage != AppStage::InGame)
		return;

	GameInstance* gameInstance = Util::getGameInstance();

	EntityStoreModule* entityStoreModule = gameInstance->EntityStoreModule;

	int entityCount = entityStoreModule->entityCount;
	Array<Entity*>* entityArray = entityStoreModule->entityArray;


	//NONE OF THIS WORKS
	for (EntityData& data : SDK::entities) {
		if (!data.entityPtr)
			continue;
		Entity* entity = data.entityPtr;

		if (entity->entityType != Entity::Item)
			continue;

		Vector3 offset =
			Util::getLocalPlayer()->Position - entity->Position;

		BoundingBox box;
		box.min = offset;
		box.max = offset + Vector3(0.1, 1, 0.1);

		entity->DefaultHitbox = box;
		entity->Hitbox = box;
		entity->Position = Util::getLocalPlayer()->Position;
		entity->RenderPos = Util::getLocalPlayer()->Position;
			
	}
}

void ItemMagnet::Initialize() {
	Util::log("Initialized ItemMagnet feature");
	RegisterEvent(this);
}