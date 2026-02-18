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
	if (event.GetEventType() == Event::EventType::MoveCycleEvent) {
		for (auto& feature : features) {
			if (feature->CanExecute() && feature->active)
				feature->PlayerMove(static_cast<MoveCycleEvent&>(event));
		}
		return;
	}

	if (event.GetEventType() == Event::EventType::Render3DEvent) {
		for (auto& feature : features) {
			if (feature->CanExecute() && feature->active)
				feature->OnRender3D(static_cast<Render3DEvent&>(event));
		}
		return;
	}

	if (event.GetEventType() == Event::EventType::Render2DEvent) {
		for (auto& feature : features) {
			if (feature->CanExecute() && feature->active)
				feature->OnRender2D(static_cast<Render2DEvent&>(event));
		}
		return;
	}

}

void FeatureDispatcher::TickAll() {
	for (auto& feature : features) {
		if (feature->CanExecute() && feature->active)
			feature->OnTick();
	}
}