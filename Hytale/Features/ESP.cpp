#include "ESP.h"

#include "../Renderer/Renderer3D.h"
#include "../Renderer/FontRenderer/Fonts.h"
#include "../Util/Util.h"
#include "core.h"

ESP::ESP() : Feature("ESP") {
	this->toggle = this->RegisterSetting<ToggleSetting>("Self", false);
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
		
		renderer3D.renderer3D.BoxLines(entity.entityPtr, Color::Normalize(50, 50, 255, 100), Color::Normalize(50, 50, 255, 255));
		
	}
}

bool ESP::CanExecute() {
	ValidPtrBool(Util::getLocalPlayer());
}