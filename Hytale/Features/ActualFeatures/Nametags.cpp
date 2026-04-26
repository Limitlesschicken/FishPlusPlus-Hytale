/*
 * Copyright (c) FishPlusPlus.
 */
#include "Nametags.h"

#include "core.h"

Nametags::Nametags() : Feature("Nametags") {
	this->NPCTags = this->RegisterSetting<ToggleSetting>("Show NPCs", true);
}

void Nametags::OnRender3D(Renderer3D& renderer3D) {
	SDK::global_mutex.lock();
	std::vector<EntityData> entities = SDK::entities;
	SDK::global_mutex.unlock();

	for (const auto& entity : entities) {
		Vector2 screenPos;
		ValidPtrLoop(entity.entityPtr);
		if (entity.isLocalPlayer)
			continue;

		if (entity.entityType != Entity::Character)
			continue;

		if (!entity.player && !this->NPCTags->GetValue())
			continue;

		if (!Util::WorldToScreen(entity.entityPtr->RenderPos + Vector3(0, entity.entityPtr->DefaultHitbox.max.y, 0), screenPos))
			continue;

		float health = entity.entityPtr->DisplayHealth * 100.0f;
		Color healthColor = Color::Green();
		if (health <= 50.0f)
			healthColor = Color(255, 255, 0);
		if (health <= 20.0f)
			healthColor = Color::Red();

		std::string healthString = Util::string_format(" %.1f", health);

		Fonts::Figtree->RenderTextShadow(entity.name, screenPos.x - Fonts::Figtree->getWidth(entity.name) / 2, screenPos.y, 1, Color::White());
		Fonts::Figtree->RenderText(healthString, (screenPos.x - Fonts::Figtree->getWidth(entity.name) / 2) + Fonts::Figtree->getWidth(entity.name), screenPos.y, 1, healthColor);

	}
}

void Nametags::Initialize() {
	Util::log("Initialized Nametags feature");
	RegisterEvent(this);
}