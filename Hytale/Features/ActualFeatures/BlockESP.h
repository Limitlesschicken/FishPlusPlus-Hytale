/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Features/Feature.h"

#include "Features/Settings/SliderSetting.h"
#include "Features/Settings/ToggleSetting.h"
#include "Features/Settings/BlockSelection/BlockSetting.h"

class BlockESP : public Feature {
public:
	BlockESP();
	void OnRender3D(Renderer3D& renderer3D);
	bool CanExecute() override;
	void Initialize() override;

	bool IsBlockImportant(int settingID);

	BlockSetting* blockSetting;

	SliderSetting* radius;
	ToggleSetting* showName;
	ToggleSetting* showChests;
	ToggleSetting* showBenches;
	ToggleSetting* showAdamantite;
	ToggleSetting* showGold;
	ToggleSetting* showSilver;
	ToggleSetting* showIron;
	ToggleSetting* showCopper;
	ToggleSetting* showMithril;
	ToggleSetting* showCobalt;
	ToggleSetting* showThorium;
	ToggleSetting* showTreasure;
	ToggleSetting* showCrystals;
	ToggleSetting* showGems;
	ToggleSetting* refreshList;
};
