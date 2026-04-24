/*
 * Copyright (c) FishPlusPlus.
 */
#include "FeatureHandler.h"

#include "../Menu/Menu.h"
#include "../Menu/Children/Tab.h"

#include "ActualFeatures/Speed.h"
#include "ActualFeatures/Flight.h"
#include "ActualFeatures/NameTags.h"
#include "ActualFeatures/ESP.h"
#include "ActualFeatures/NoFall.h"
#include "ActualFeatures/WorldModulate.h"
#include "ActualFeatures/Outline.h"
#include "ActualFeatures/BlockESP.h"
#include "ActualFeatures/Tracers.h"
#include "ActualFeatures/Scaffold.h"
#include "ActualFeatures/HitBox.h"
#include "ActualFeatures/Gamemode.h"
#include "ActualFeatures/ItemMagnet.h"
#include "ActualFeatures/RemoteChest.h"
#include "ActualFeatures/NoDeath.h"
#include "ActualFeatures/NoMapClear.h"
#include "ActualFeatures/Hud.h"
#include "ActualFeatures/Hud/WaterMark.h"
#include "ActualFeatures/Hud/Position.h"
#include "ActualFeatures/Hud/FeatureList.h"

void InitFeature(std::unique_ptr<Feature> feature, std::string tab) {
	feature->setCategory(tab);
	feature->CreateForcedKeybind();
	feature->Initialize();

	FeatureHandler::features.push_back(std::move(feature));
}

void FeatureHandler::Init() {
	InitFeature(std::make_unique<HitBox>(), "Combat");
	InitFeature(std::make_unique<NoDeath>(), "Combat");

	InitFeature(std::make_unique<Flight>(), "Movement");
	InitFeature(std::make_unique<Speed>(), "Movement");
	InitFeature(std::make_unique<NoFall>(), "Movement");
	InitFeature(std::make_unique<Scaffold>(), "Movement");
	InitFeature(std::make_unique<Gamemode>(), "Movement");

	InitFeature(std::make_unique<ESP>(), "Visuals");
	InitFeature(std::make_unique<Nametags>(), "Visuals");
	InitFeature(std::make_unique<WorldModulate>(), "Visuals");
	InitFeature(std::make_unique<Outline>(), "Visuals");
	InitFeature(std::make_unique<BlockESP>(), "Visuals");
	InitFeature(std::make_unique<Tracers>(), "Visuals");

	InitFeature(std::make_unique<RemoteChest>(), "Misc");
	//InitFeature(std::make_unique<NoMapClear>(), "Misc");

	//InitFeature(std::make_unique<ItemMagnet>(), "Misc");

	InitFeature(std::make_unique<Hud>(), "Client");


	//HUD Features
	InitFeature(std::make_unique<WaterMark>(), "Hud");
	InitFeature(std::make_unique<PositionHud>(), "Hud");
	InitFeature(std::make_unique<FeatureList>(), "Hud");

	Menu::mainComponent->AddChild(std::make_unique<Tab>("Combat", 200, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Movement", 460, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Visuals", 720, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Misc", 980, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Client", 1240, 200));
	//Menu::mainComponent->AddChild(std::make_unique<Tab>("Misc", 830, 200, 300));

	Menu::hudTabComponent->AddChild(std::make_unique<Tab>("Hud", 200, 200));
}

bool FeatureHandler::FeaturesLoaded() {
	return !features.empty();
}

Feature* FeatureHandler::GetFeatureFromName(std::string name) {
	for (auto& feature : features) {
		if (name == feature->GetName())
			return feature.get();
	}
	return nullptr;
}