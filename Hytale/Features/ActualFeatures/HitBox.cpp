/*
 * Copyright (c) FishPlusPlus.
 */
#include "HitBox.h"

HitBox::HitBox() : Feature("Hitbox") {
	this->players = this->RegisterSetting<ToggleSetting>("Players", true);
	this->mobs = this->RegisterSetting<ToggleSetting>("Mobs", true);
};

void HitBox::OnFrame() {
	for (EntityData& data : SDK::entities) {
		if (!data.entityPtr)
			continue;

		if (data.isLocalPlayer)
			continue;

		if (data.entityType != Entity::Character)
			continue;
		
		Entity* entity = data.entityPtr;

		if (!entity)
			continue;

		bool isPlayer = entity->IsAPlayer();
		bool shouldAffect = (isPlayer && this->players->GetValue()) || (!isPlayer && this->mobs->GetValue());

		if (!entityHotboxSave.contains(entity)) {
			entityHotboxSave[entity] = entity->Hitbox;
		}

		if (shouldAffect) {
			Vector3 offset =
				(Util::getLocalPlayer()->Position + Vector3(0, 1, 0)) - entity->Position;

			BoundingBox box;
			box.min = offset - Vector3(0.1, 0, 0.1);
			box.max = offset + Vector3(0.1, 1, 0.1);

			entity->Hitbox = box;
		}
		else {
			if (entity) {
				entity->Hitbox = entityHotboxSave[entity];
				entity->DefaultHitbox = entityHotboxSave[entity];
			}
		}
	}
}

void HitBox::OnDeactivate() {
	if (Util::app->Stage != AppStage::InGame)
		return;

	for (EntityData& data : SDK::entities) {
		if (!data.entityPtr)
			continue;

		auto it = entityHotboxSave.find(data.entityPtr);

		if (it != entityHotboxSave.end()) {
			auto& box= it->second;
			Entity* entity = it->first;
			if (!entity)
				continue;
			entity->DefaultHitbox = box;
		}
	}

	entityHotboxSave.clear();
}

void HitBox::Initialize() {
	Util::log("Initialized Hitbox feature");
	RegisterEvent(this);
}