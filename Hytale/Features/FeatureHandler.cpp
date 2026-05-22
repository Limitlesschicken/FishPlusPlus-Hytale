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
#include "ActualFeatures/RemoteChest.h"
#include "ActualFeatures/NoDeath.h"
#include "ActualFeatures/Hud.h"
#include "ActualFeatures/Hud/WaterMark.h"
#include "ActualFeatures/Hud/Position.h"
#include "ActualFeatures/Hud/FeatureList.h"
#include "ActualFeatures/Hud/PlayerList.h"
#include "ActualFeatures/MainMenuEffect.h"
#include "ActualFeatures/Hud/Addresses.h"

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
	

	InitFeature(std::make_unique<ESP>(), "Visuals");
	InitFeature(std::make_unique<Nametags>(), "Visuals");
	InitFeature(std::make_unique<WorldModulate>(), "Visuals");
	InitFeature(std::make_unique<Outline>(), "Visuals");
	InitFeature(std::make_unique<BlockESP>(), "Visuals");
	InitFeature(std::make_unique<Tracers>(), "Visuals");

	InitFeature(std::make_unique<RemoteChest>(), "Misc");
	InitFeature(std::make_unique<Gamemode>(), "Misc");
	

	InitFeature(std::make_unique<Hud>(), "Client");
	InitFeature(std::make_unique<MainMenuEffect>(), "Client");


	//HUD Features
	InitFeature(std::make_unique<WaterMark>(), "Hud");
	InitFeature(std::make_unique<PositionHud>(), "Hud");
	InitFeature(std::make_unique<FeatureList>(), "Hud");
	InitFeature(std::make_unique<PlayerList>(), "Hud");
	InitFeature(std::make_unique<Addresses>(), "Hud");

	Menu::mainComponent->AddChild(std::make_unique<Tab>("Combat", 200, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Movement", 460, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Visuals", 720, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Misc", 980, 200));
	Menu::mainComponent->AddChild(std::make_unique<Tab>("Client", 1240, 200));

	Menu::hudTabComponent->AddChild(std::make_unique<Tab>("Hud", 200, 200));
}

bool FeatureHandler::FeaturesLoaded() {
	return !features.empty();
}