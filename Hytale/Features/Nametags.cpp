#include "Nametags.h"

#include "../Renderer/Renderer3D.h"
#include "../Renderer/FontRenderer/Fonts.h"
#include "../Util/Util.h"

void Nametags::OnRender3D(Render3DEvent& renderer3D) {
	for (const auto& entity : Util::getEntities()) {
		if (entity->entityPlayerType != Entity::EntityPlayerType::Player)
			continue;

		Vector2 screenPos;
		if (Util::WorldToScreen(entity->RenderPos + Vector3(0.0f, 2.1f, 0.0f), screenPos)) {
			Fonts::Figtree->RenderText(entity->Name->getName(), screenPos.x - Fonts::Figtree->getWidth(entity->Name->getName()) / 2, screenPos.y, 1, Color::White());
		}
	}
}

bool Nametags::CanExecute() {
	return (Util::getLocalPlayer() != nullptr);
}