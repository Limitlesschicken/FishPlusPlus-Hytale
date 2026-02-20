/*
 * Copyright (c) FishPlusPlus.
 */
#include "FeatureDispatcher.h"

#include "../Menu/Menu.h"
#include "../Menu/Children/Tab.h"

#include "../Features/Flight.h"
#include "../Features/ESP.h"
#include "../Features/Nametags.h"
#include "../Features/Speed.h"
#include "../Features/NoFall.h"

std::vector<std::unique_ptr<Feature>> FeatureDispatcher::features;

void InitFeature(std::unique_ptr<Feature> feature, std::string tab) {
	feature->setCategory(tab);
	//Create keybind here so its forced at the back of the list. If theres a better way to do this lmk
	feature->CreateForcedKeybind();
	FeatureDispatcher::features.push_back(std::move(feature));
}
void FeatureDispatcher::initFeatures() {
	InitFeature(std::make_unique<Flight>(), "Movement");
	InitFeature(std::make_unique<Speed>(), "Movement");
	InitFeature(std::make_unique<NoFall>(), "Movement");
	InitFeature(std::make_unique<ESP>(), "Visuals");
	InitFeature(std::make_unique<Nametags>(), "Visuals");

	Menu::mainComponent->AddChild(std::make_unique<Tab>("Movement", 200, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Visuals", 410, 200));
}

void FeatureDispatcher::DispatchEvent(Event& event) {
	for (auto& feature : features) {
		if (!feature->CanExecute() || !feature->IsActive()) {
			continue;
		}
		switch (event.GetEventType()) {
		case Event::EventType::MoveCycleEvent:
			feature->PlayerMove(static_cast<MoveCycleEvent&>(event));
			break;
		case Event::EventType::Render3DEvent:
			feature->OnRender3D(static_cast<Render3DEvent&>(event));
			break;
		case Event::EventType::Render2DEvent:
			feature->OnRender2D(static_cast<Render2DEvent&>(event));
			break;
		default:
			break;
		}
	}
}

void FeatureDispatcher::TickAll() {
	for (auto& feature : features) {
		if (feature->CanExecute() && feature->IsActive())
			feature->OnTick();
	}
}