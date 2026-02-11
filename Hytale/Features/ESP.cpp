#include "ESP.h"

#include "../Renderer/Renderer3D.h"
#include "../Renderer/FontRenderer/Fonts.h"
#include "../Util/Util.h"

#include "FeatureDispatcher/Settings/ToggleSetting.h"
#include "FeatureDispatcher/Settings/SliderSetting.h"
#include "FeatureDispatcher/Settings/MultiSetting.h"

ToggleSetting* toggle;
SliderSetting* testSlider;
MultiSetting* testMulti;

ESP::ESP() : Feature("ESP") {
	toggle = this->RegisterSetting<ToggleSetting>("Self", false);
	testSlider = this->RegisterSetting<SliderSetting>("HAHA", 50.0f, 0.0f, 100.0f);
	testMulti = this->RegisterSetting<MultiSetting>("testMulti", std::vector<std::string>{ "one", "two", "three", "four" }, 2);
}

void ESP::OnRender3D(Render3DEvent& renderer3D) {
	for (const auto& entity : Util::getEntities()) {
		if (entity->entityType != Entity::EntityType::Character && entity->entityType != Entity::EntityType::Item)
			continue;

		if (toggle->GetValue() && (uintptr_t)entity == (uintptr_t)Util::getLocalPlayer())
			continue;
		
		renderer3D.renderer3D.BoxLines(entity, Color::Normalize(50, 50, 255, testSlider->GetValue()), Color::Normalize(50, 50, 255, 255));
		renderer3D.renderer3D.Render();
	}
}

bool ESP::CanExecute() {
	return (Util::getLocalPlayer() != nullptr);
}