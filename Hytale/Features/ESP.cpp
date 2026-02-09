#include "ESP.h"

#include "../Renderer/Renderer3D.h"
#include "../Renderer/FontRenderer/Fonts.h"
#include "../Util/Util.h"

void ESP::OnRender3D(Render3DEvent& renderer3D) {
	for (const auto& entity : Util::getEntities()) {
		if (entity->entityType != Entity::EntityType::Character && entity->entityType != Entity::EntityType::Item)
			continue;

		if ((uintptr_t)entity == (uintptr_t)Util::getLocalPlayer())
			continue;
		
		renderer3D.renderer3D.BoxLines(entity, Color::From255(50, 50, 255, 50), Color::From255(50, 50, 255, 255));
		renderer3D.renderer3D.Render();
	}
}

bool ESP::CanExecute() {
	return (Util::getLocalPlayer() != nullptr);
}