/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Features/Feature.h"

#include "Features/Settings/SliderSetting.h"
#include "Features/Settings/ToggleSetting.h"
#include "Features/Settings/BlockSelection/BlockSetting.h"
#include "Features/Settings/KeybindSetting.h"
#include "Math/Vector3.h"
#include <vector>
#include <string>

struct SavedChest {
	Vector3 position;
	std::string name;
	bool selected;

	SavedChest(Vector3 pos, const std::string& chestName)
		: position(pos), name(chestName), selected(false) {}
};

class RemoteChest : public Feature {
public:
	RemoteChest();
	void OnRender3D(Renderer3D& renderer3D);
	bool CanExecute() override;
	void Initialize() override;

	ToggleSetting* ShowTarget;
	ToggleSetting* ShowNames;
	SliderSetting* MaxDistance;
	KeybindSetting* SaveChestKeybind;
	KeybindSetting* OpenSelectedChestKeybind;
	KeybindSetting* NextChestKeybind;
	KeybindSetting* PrevChestKeybind;

	std::vector<SavedChest> savedChests;
	int selectedChestIndex = -1;

	void AddOrRemoveChest(const Vector3& pos, const std::string& blockName);
	int FindChestIndex(const Vector3& pos);
	void SelectNextChest();
	void SelectPrevChest();
	void OpenSelectedChest();
	void SaveChests();
	void LoadChests();
};
