/*
 * Copyright (c) FishPlusPlus.
 */
#include "ESP.h"

#include "core.h"
#include <FeatureDispatcher/Settings/ColorSetting.h>

ESP::ESP() : Feature("ESP") {
	this->toggle = this->RegisterSetting<ToggleSetting>("Self", false);
	this->insideColor = this->RegisterSetting<ColorSetting>("Inside", Color(50, 50, 255, 100));
	this->outsideColor = this->RegisterSetting<ColorSetting>("Outside", Color(50, 50, 255, 255));

	this->testRecursive = this->RegisterSetting<RecursiveSetting>("testRecursive", false);

	this->colorUnderRecrusive = this->testRecursive->RegisterSetting<ColorSetting>("testColor", Color(255, 255, 0, 255));
}

void ESP::OnRender3D(Render3DEvent& renderer3D) {
	SDK::global_mutex.lock();
	std::vector<EntityData> entities = SDK::entities;
	SDK::global_mutex.unlock();

	for (const auto& entity : entities) {
		if (entity.entityType != Entity::EntityType::Character && entity.entityType != Entity::EntityType::Item)
			continue;

		if (!toggle->GetValue() && entity.isLocalPlayer)
			continue;
		
		renderer3D.renderer3D.BoxLines(entity.entityPtr, Color::Normalize(insideColor->GetValue()), Color::Normalize(outsideColor->GetValue()));
	}
}

bool ESP::CanExecute() {
	ValidPtrBool(Util::getLocalPlayer());
}