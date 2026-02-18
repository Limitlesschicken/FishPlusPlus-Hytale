#include "ESP.h"

#include "../Renderer/Renderer3D.h"
#include "../Renderer/FontRenderer/Fonts.h"
#include "../Util/Util.h"

ESP::ESP() : Feature("ESP") {
	this->toggle = this->RegisterSetting<ToggleSetting>("Self", false);
}

void ESP::OnRender3D(Render3DEvent& renderer3D) {
	for (const auto& entity : Util::getEntities()) {
		if (entity->entityType != Entity::EntityType::Character && entity->entityType != Entity::EntityType::Item)
			continue;

		if (!toggle->GetValue() && (uintptr_t)entity == (uintptr_t)Util::getLocalPlayer())
			continue;
		
		renderer3D.renderer3D.BoxLines(entity, Color::Normalize(50, 50, 255, 100), Color::Normalize(50, 50, 255, 255));
		
	}
}

bool ESP::CanExecute() {
	return (Util::getLocalPlayer() != nullptr);
}