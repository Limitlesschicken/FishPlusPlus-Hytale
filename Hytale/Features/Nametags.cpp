#include "Nametags.h"

#include "core.h"
#include "../Renderer/Renderer3D.h"
#include "../Renderer/FontRenderer/Fonts.h"
#include "../Util/Util.h"

void Nametags::OnRender3D(Render3DEvent& renderer3D) {
	SDK::global_mutex.lock();
	std::vector<EntityData> entities = SDK::entities;
	SDK::global_mutex.unlock();

	for (const auto& entity : entities) {
		Vector2 screenPos;
		if (!Util::WorldToScreen(entity.entityPtr->RenderPos + Vector3(0.0f, 2.1f, 0.0f), screenPos))
			continue;

		Fonts::Figtree->RenderText(entity.name, screenPos.x - Fonts::Figtree->getWidth(entity.name) / 2, screenPos.y, 1, Color::White());

	}
}

bool Nametags::CanExecute() {
	ValidPtrBool(Util::getLocalPlayer());
}